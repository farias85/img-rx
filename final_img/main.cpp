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

#include <iostream>
#include <string>
#include <math.h>
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>

#include "Format.h"

using namespace std;
using namespace cv;
using namespace img;

/**
* Abre una ventana para visualizar una imagen.
* @param image Matriz de la imagen.
*/
void verImagen(const Mat &image);

/**
* Convierte los arreglos bidimensionales de los canales RGB de la clase Format en una matriz Mat de 3 canales de opencv.
* @param mat Matriz resultado.
* @param f1 Instancia de Format con el campo de datos de la imagen del archivo IMG previamente cargado.
*/
void convertArrayRGB2Mat(Mat &mat, const Format &f1);

int main()
{	
	vector<char*> vstr;
	//vstr.push_back("C:/sample/manual.IMG");
	vstr.push_back("C:/sample/BAG1.IMG");
	vstr.push_back("C:/sample/S7.IMG");
	vstr.push_back("C:/sample/THS20080001_100000003.IMG");
	vstr.push_back("C:/sample/80001_20120702_00001043.IMG");
	vstr.push_back("C:/sample/000001_20131029_00062296.IMG");
	vstr.push_back("C:/sample/000001_20131029_00062285.IMG");
	vstr.push_back("C:/sample/000001_20131022_00020280.IMG");
	vstr.push_back("C:/sample/000001_20131017_00002504.IMG");
	vstr.push_back("C:/sample/000001_20131017_00002499.IMG");

	try
	{
		for(int i = 0; i < vstr.size(); i++)
		{
			char *dir = vstr[i];
			cout << endl;
			//Creando el objeto con la direcci&oacute;n del archivo IMG.
			Format fx(dir);

			cout << dir << endl;
			cout << (short)fx.getFormatType() << endl;

			//Chequeando que el formato del archivo es v&aacute;lido
			if(fx.getFormatType() != 0)
			{
				//Si el formato es v&aacute;lido, se cargan los componentes de la cabecera primeramente
				fx.loadHeaderData();
				cout << fx.getModel() << endl;

				//Luego de cargada la cabecera, se cargan los datos de la imagen
				fx.loadImageData();

				//Inicializando la matriz que voy a enviar como parametro
				Mat mtxRGBFinal(fx.getHeight(), fx.getWidth(), CV_8UC3, Scalar(255, 255, 255));

				convertArrayRGB2Mat(mtxRGBFinal, fx);

				verImagen(mtxRGBFinal);
			}
			else
			{
				cout << "formato no válido";
			}
		}
	}
	catch(invalid_argument &e)
	{
		cout << e.what() << endl;
	}
	catch(...)
	{
		cout << "Error" <<endl;
	}
}

void convertArrayRGB2Mat(Mat &mat, const Format &f1)
{
	for(int k = 0; k < f1.getHeight(); k++)
	{
		for(int j = 0; j < f1.getWidth(); j++)
		{
			double valueB = f1.getChannelB()[k][j];
			double valueG = f1.getChannelG()[k][j];
			double valueR = f1.getChannelR()[k][j];

			mat.at<Vec3b>(k, j)[0] = valueB;
			mat.at<Vec3b>(k, j)[1] = valueG;
			mat.at<Vec3b>(k, j)[2] = valueR;
		}
	}
}

void verImagen(const Mat &image)
{	
	string winName = "Final Image";
	namedWindow(winName);
	imshow(winName, image);

	waitKey();
	destroyWindow(winName);
}