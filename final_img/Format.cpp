/**
* Licensed under the Apache License, Version 2.0 (the "License"); you may not
* use this file except in compliance with the License. You may obtain a copy of
* the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
* WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
* License for the specific language governing permissions and limitations under
* the License.
*
* Created by Felipe Rodriguez Arias <ucifarias@gmail.com>.
*/

#include "Format.h"

namespace img
{
	Format::Format(const char *fullImageName) :startIndex(0), width(0), height(0) {

		byteArray = file2ByteVector(fullImageName);
		whatFormat();
	}

	Format::~Format(void)
	{
		for(int k = 0; k < height; k++)
		{	
			delete[] array2dB[k];
			delete[] array2dG[k];
			delete[] array2dR[k];
		}

		/*delete[] array2dB;
		delete[] array2dG;
		delete[] array2dR;*/
	}

	void Format::whatFormat()
	{
		int const count = 8;
		string id = getString(1, count + 1);
		formatType = -1;	
		short firma[count] = {0, 0, 44, 0, 0, 0, -81, 0};

		bool valid = true;
		for(int i = 0; i < sizeof(firma)/sizeof(firma[0]); i++)
		{
			if(id[i] != firma[i])
			{
				valid = false;
				break;
			}
		}	

		formatType = !valid == 0 ? (byte)this->read1Byte(23) > 100 ? 1 : 2 : 0;	
	}

	vector<byte> Format::file2ByteVector(const char *dir){

		ifstream file(dir, ios::binary);
		file.seekg(0, ios::end);
		size_t fileSize = file.tellg();
		file.seekg(0, ios::beg);

		vector<byte> data(fileSize, 0);
		file.read(reinterpret_cast<char*>(&data[0]), fileSize);

		file.close();

		return data;
	}

	void Format::loadHeaderData()
	{
		switch(formatType)
		{
		case 1:
			{
				width = (int)this->read2Bytes(649);
				height = (int)this->read2Bytes(651);
				sequenceNumber = this->read4Bytes(661);
				model = this->getString(333, 344);
				date = this->getString(349, 368);
				systemId = this->getString(383, 393);
				dataBytes = this->read4Bytes(641);
				imageBytes = this->read4Bytes(657);
				rayIntensity = this->read2Bytes(799) / 10.0;
				startIndex = 825;
			}
			break;
		case 2:
			{
				width = (int)this->read2Bytes(69);
				height = (int)this->read2Bytes(71);
				sequenceNumber = 0.0;
				model = this->getString(43, 51);
				date = this->getString(58, 78);
				systemId = this->getString(383, 393);
				dataBytes = this->read4Bytes(61);
				imageBytes = this->read4Bytes(77);

				startIndex = 245;
			}
			break;
		default:
			throw invalid_argument("Invalid IMG format.");
		}
	}

	void Format::loadImageData(){

		try
		{
			if(formatType == 0)
				throw invalid_argument("Invalid IMG format.");

			if(startIndex == 0)			
				throw invalid_argument("IMG header couldn't be opened.");			

			vector<byte> channel0;
			vector<byte> channel1;
			vector<byte> channel2;
			vector<byte> channel3;     	

			vector<double> channelH;	
			vector<double> channelS;
			vector<double> channelI;

			vector<ImageDataControl> idcs;
			HSIColorTable table;

			int columnActual = 0;
			int dataHeight = 0;	

			while (startIndex < byteArray.size() && columnActual < width)
			{	
				int columnLength = (int)read2Bytes(startIndex);
				int zeroPadding = (int)read2Bytes(startIndex + 2);

				if(startIndex + columnLength > byteArray.size())
					break;

				if(columnLength + zeroPadding > height)
					break;

				ImageDataControl idc(startIndex, columnLength, zeroPadding);
				idcs.push_back(idc);

				startIndex += 8; //2 bytes de CL + 2 bytes de ZP + 4 bytes de ZM = 8 bytes

				if (columnLength != 0)
				{
					int channelValues = 0;
					for (int i = 0; i < columnLength; i++)
					{
						long posAux = startIndex + channelValues;

						byte bch0 = read1Byte(posAux);
						byte bch1 = read1Byte(posAux + 1);
						byte bch2 = read1Byte(posAux + 2);
						byte bch3 = read1Byte(posAux + 3);

						channel0.push_back(bch0);
						channel1.push_back(bch1);
						channel2.push_back(bch2);
						channel3.push_back(bch3);

						int ch1I = bch1 << 3;
						int ch0I = bch0 >> 5;

						int indexI = ch1I + ch0I;				
						double valueI = table.iCurve[indexI];
						channelI.push_back(valueI); 

						channelH.push_back(table.hCurve[bch3]);

						int bch3Control = 32;
						bch3 = bch3 > bch3Control ? bch3Control : bch3;

						int indexS = (indexI) * (bch3Control + 1)  + bch3;
						double valueS = table.sMatrix[indexS];
						channelS.push_back(valueS);

						channelValues += 4;
					}
				}

				startIndex += columnLength * 4; //each CL's column contains 4 bytes

				if (dataHeight < columnLength + zeroPadding)
					dataHeight = columnLength + zeroPadding;

				columnActual++;
			}

			//Inicializo los valores de los arrays HSI
			height = dataHeight == 0 ? height : dataHeight;

			double **array2dH = new double*[height];
			double **array2dS = new double*[height];
			double **array2dI = new double*[height];
			double **resultI = new double*[height];

			array2dR = new double*[height];
			array2dG = new double*[height];
			array2dB = new double*[height];

			for(int k = 0; k < height; k++)
			{
				array2dH[k] = new double[width];
				array2dS[k] = new double[width];
				array2dI[k] = new double[width];
				resultI[k] = new double[width];

				array2dR[k] = new double[width];
				array2dG[k] = new double[width];
				array2dB[k] = new double[width];

				for(int j = 0; j < width; j++) 
				{	
					array2dH[k][j] = 1;		
					array2dS[k][j] = 1;
					array2dI[k][j] = 1;
					resultI[k][j] = 1;

					array2dR[k][j] = 255;
					array2dG[k][j] = 255;
					array2dB[k][j] = 255;
				}
			}	

			int globalCount = 0;
			int idcsSize = idcs.size();

			//Transformando los valores que estaban en los vectores secuencialmente hacia las matrices (arreglos bidimensionales) HSI
			for(int k = 0; k < idcsSize; k++)
			{	
				int zeroPadding = idcs[k].getZeroPadding();
				int columnLength = idcs[k].getColumnLength();

				for (int i = zeroPadding; i < columnLength + zeroPadding; i++)
				{	
					array2dH[i][k] = channelH[globalCount];
					array2dS[i][k] = channelS[globalCount];
					array2dI[i][k] = channelI[globalCount];

					globalCount++;
				}
			}

			//Filtrando el canal de intensidad para realzar bordes
			imgFilter2D(array2dI, resultI);	

			//Convirtiendo los valores de HSI a RGB
			for(int k = 0; k < idcsSize; k++)
			{	
				int zeroPadding = idcs[k].getZeroPadding();
				int columnLength = idcs[k].getColumnLength();

				for (int i = zeroPadding; i < columnLength + zeroPadding; i++)
				{	
					double valueR = convertHSI2RGB(array2dH[i][k], array2dS[i][k], resultI[i][k])[0];
					double valueG = convertHSI2RGB(array2dH[i][k], array2dS[i][k], resultI[i][k])[1];
					double valueB = convertHSI2RGB(array2dH[i][k], array2dS[i][k], resultI[i][k])[2];

					array2dR[i][k] = valueR;
					array2dG[i][k] = valueG;
					array2dB[i][k] = valueB;
				}
			}

			//Eliminando los punteros utilizados
			for(int k = 0; k < height; k++)
			{	
				delete[] array2dH[k];
				delete[] array2dS[k];
				delete[] array2dI[k];
				delete[] resultI[k];
			}

			delete[] array2dH;
			delete[] array2dS;
			delete[] array2dI;
			delete[] resultI;
		}
		catch(invalid_argument &e)
		{
			throw invalid_argument(e.what());
		}
		catch(...)
		{
			throw invalid_argument("IMG image could not be load.");
		}
	}

	void Format::imgFilter2D(double** &image, double** &result) 
	{ 
		int const filterWidth = 3; 
		int const filterHeight = 7; 

		double filter[filterWidth][filterHeight] =  
		{ 
			-0.010561056105611,   0.190099009900990,  -0.359075907590759,  -1.351815181518151,   0.517491749174917,  -0.052805280528053, 0,
			0.039273927392740,  -0.706930693069306,   1.335313531353137,   5.027062706270626,  -1.924422442244224,   0.196369636963698, 0,
			-0.018811881188119,   0.338613861386138,  -0.639603960396040,  -2.407920792079207,   0.921782178217821,  -0.094059405940595, 0
		}; 

		//apply the filter 
		for(int x = 0; x < height; x++) 
			for(int y = 0; y < width; y++) 
			{ 
				double value = 0.0; 

				//multiply every value of the filter with corresponding image pixel 
				for(int filterX = 0; filterX < filterWidth; filterX++) 
					for(int filterY = 0; filterY < filterHeight; filterY++) 
					{ 
						int imageX = (x - filterWidth / 2 + filterX + height) % height; 
						int imageY = (y - filterHeight / 2 + filterY + width) % width; 

						value += image[imageX][imageY] * filter[filterX][filterY]; 
					} 

					result[x][y] = value < 0 ? 0 : value;
					result[x][y] = value > 1 ? 1 : value;
			}    
	}

	double* Format::convertHSI2RGB(double valueH, double valueS, double valueI)
	{
		const double PI = std::atan(1.0) * 4;

		double h = valueH * 2 * PI;
		double s = valueS;
		double i = valueI;

		double r = 0, g = 0, b = 0;

		if(h >= 0 && h < 2 * PI / 3)
		{
			b = i * (1 - s);
			r = i * (1 + s * cos(h) / cos(PI / 3 - h));
			g = 3 * i - (r + b);
		}
		else if(h >= 2 * PI / 3 && h < 4 * PI / 3)
		{
			r = i * (1 - s);
			g = i * (1 + s * cos(h - 2 * PI / 3) / cos(PI - h));
			b = 3 * i - (r + g);
		}
		else
		{
			g = i * (1 - s);
			b = i * (1 + s * cos(h - 4 * PI / 3) / cos(5 * PI / 3 - h));
			r = 3 * i - (g + b);
		}	

		//controlando que no se excedan los limites entre 0 y 1
		r = r > 1 ? 1 : r;
		r = r < 0 ? 0 : r;

		g = g > 1 ? 1 : g;
		g = g < 0 ? 0 : g;

		b = b > 1 ? 1 : b;
		b = b < 0 ? 0 : b;

		//llevando los valores al intervalo entre 0 y 255
		r *= 255;
		g *= 255;
		b *= 255;

		double result[3] = {r, g, b};

		return result;
	}

	short Format::read1Byte(int byteNum) {

		return byteNum >= byteArray.size() ? 0 : (short)byteArray[byteNum - 1];
	}

	double Format::read2Bytes(int byteInferior) {

		return read1Byte(byteInferior + 1) * pow(2.0, 8.0) +
			read1Byte(byteInferior);
	}

	double Format::read4Bytes(int byteInferior) {

		return read1Byte(byteInferior + 3) * pow(2.0, 24.0) +
			read1Byte(byteInferior + 2) * pow(2.0, 16.0) +
			read1Byte(byteInferior + 1) * pow(2.0, 8.0) +
			read1Byte(byteInferior);
	}

	string Format::getString(int inf, int superior) {

		string resultado = "";

		for (int i = inf; i < superior; i++)
			resultado += (char)read2Bytes(i * 2 - 1);

		return resultado;
	}
}