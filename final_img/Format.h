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

#ifndef Format_H
#define Format_H

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include <string>
#include <stdexcept>
#include "ImageDataControl.h"
#include "HSIColorTable.h"

using namespace std;
typedef unsigned char byte;

/**
* Espacio de nombres de la librer&iacute;a para la lectura del formato IMG.
*/
namespace img
{
	/**
	* Define la estructura del formato IMG.
	*/
	class Format
	{
	protected:	
		/**
		* N&uacute;mero del byte donde comienzan los datos de la imagen.
		*/
		long startIndex;

		/**
		* Identificador del tipo de formato del archivo IMG, respecto a la versi&oacute;n del software con que fue creado.
		*/
		byte formatType;

		/**
		* El ancho de la imagen.
		*/
		int width;

		/**
		* El alto de la imagen.
		*/
		int height;		
		
		/**
		* El modelo del equipo de Rayos X, en caso de que fuera generada la imagen por un equipo.
		*/
		string model;

		/**
		* La fecha en que fue creado el archivo IMG.
		*/
		string date;

		/**
		* El identificador interno del sistema por cada equipo de Rayos X. Por lo general este dato se muestra impreso en
		* la imagen BMP correspondiente al fichero IMG.
		*/
		string systemId;

		/**
		* El n&uacute;mero de secuencia que aparece impreso en la imgen BMP correspondiente al fichero IMG. 
		* S&oacute;lo para el formato 1.
		*/
		double sequenceNumber;

		/**
		* La intensidad en K/V de los rayos X del equipo, Solo para el formato 1, con versi&oacute;n de software 3.1.
		*/
		double rayIntensity;

		/**
		* La cantidad de bytes que tiene el fichero IMG desde el byte 648 para el formato 1 &oacute; 
		* desde el byte 69 para el formato 2, hasta el final del bloque de datos.
		*/
		double dataBytes;

		/**
		* La cantidad de bytes del bloque de datos que codifican la imagen del fichero IMG.
		*/
		double imageBytes;	
		
		/**
		* El arreglo bidimensional(matriz) que hace referencia al canal de Rojo de la imagen del fichero IMG.
		*/
		double **array2dR;

		/**
		* El arreglo bidimensional(matriz) que hace referencia al canal de Verde de la imagen del fichero IMG.
		*/
		double **array2dG;

		/**
		* El arreglo bidimensional(matriz) que hace referencia al canal de Azul de la imagen del fichero IMG.
		*/
		double **array2dB;

	public:
		/**
		* Identificador del tipo de formato del archivo IMG, respecto a la versi&oacute;n del software con que fue creado.
		* @return 0 si no es un formato inv&aacute;lido, 1 si es formato 1, 2 si es formato 2.
		*/
		inline byte getFormatType() const {return this->formatType;}

		/**
		* El ancho de la imagen.
		*/
		inline int getWidth() const {return this->width;}

		/**
		* El alto de la imagen.
		*/
		inline int getHeight() const {return this->height;}
		
		/**
		* El modelo del equipo de Rayos X, en caso de que fuera generada la imagen por un equipo.
		*/
		inline string getModel() const {return this->model;}

		/**
		* La fecha en que fue creado el archivo IMG.
		*/
		inline string getDate() const {return this->date;}

		/**
		* El identificador interno del sistema por cada equipo de Rayos X. Por lo general este dato se muestra impreso en
		* la imagen BMP correspondiente al fichero IMG.
		*/
		inline string getSystemId() const {return this->systemId;}

		/**
		* El n&uacute;mero de secuencia que aparece impreso en la imgen BMP correspondiente al fichero IMG. 
		* S&oacute;lo para el formato 1.
		*/
		inline double getSequenceNumber() const {return this->sequenceNumber;}

		/**
		* La intensidad en K/V de los rayos X del equipo, Solo para el formato 1, con versi&oacute;n de software 3.1.
		*/
		inline double getRayIntensity() const {return this->rayIntensity;}		

		/**
		* La cantidad de bytes que tiene el fichero IMG desde el byte 648 para el formato 1 &oacute; 
		* desde el byte 69 para el formato 2, hasta el final del bloque de datos.
		*/
		inline double getDataBytes() const {return this->dataBytes;}

		/**
		* La cantidad de bytes del bloque de datos que codifican la imagen del fichero IMG.
		*/
		inline double getImageBytes() const {return this->imageBytes;}	

		/**
		* El arreglo bidimensional(matriz) que hace referencia al canal de Rojo de la imagen del fichero IMG.
		*/
		inline double **getChannelR() const {return this->array2dR;}

		/**
		* El arreglo bidimensional(matriz) que hace referencia al canal de Verde de la imagen del fichero IMG.
		*/
		inline double **getChannelG() const {return this->array2dG;}

		/**
		* El arreglo bidimensional(matriz) que hace referencia al canal de Azul de la imagen del fichero IMG.
		*/
		inline double **getChannelB() const {return this->array2dB;}		

	protected:
		/**
		* Carga en un vector de bytes el archivo de la direcci&oacute;n dada.
		* @param dir La direcci&oacute;n del archivo IMG.
		* @return Un vector con todos los bytes del archivo IMG.
		*/
		vector<byte> file2ByteVector(const char* dir);

		/**
		* Aplica el filtro correspondiente de realce de bordes a la imagen pasada por par&aacute;metro
		* @param image La matriz de la imagen que se desea pasar el filtro.
		* @param result La matriz resultante de la aplicaci&oacute;n del filtro.
		*/
		void imgFilter2D(double** &image, double** &result);

		/**
		* Convierte a RGB los 3 valores de un pixel de una imagen en formato HSI.
		* @param valueH El valor de la componente de mat&iacute;z de la imagen en formato HSI.
		* @param valueS El valor de la componente de saturaci&oacute;n de la imagen en formato HSI.
		* @param valueI El valor de la componente de intensidad de la imagen en formato HSI.
		* @return Un arreglo de tres valores, [0] = Rojo, [1] = Verde, [2] = Azul.
		*/
		double* convertHSI2RGB(double valueH, double valueS, double valueI);

		/**
		* Busca identificador del tipo de formato del archivo IMG, respecto a la versi&oacute;n del software con que fue creado.
		* Modifica el valor del atributo formatType a 0 si no es un formato inv&aacute;lido, 1 si es formato 1, 2 si es formato 2.
		* El m&eacute;todo getFormatType() debe ser llamado luego de la creaci&oacute;n del objeto para comprobar que se ha cargado un 
		* fichero IMG v&aacute;lido.
		* @see getFormatType()
		*/
		void whatFormat();

	public:
		/**
		* Vector con los bytes del archivo IMG cargado.
		*/
		vector<byte> byteArray;

		/**
		* Constructor de la clase Format. Luego de creado el objeto a trav&eacute;s de este constructor
		* se debe llamar el m&eacute;todo getFormatType() para comprobar que se ha cargado un 
		* fichero IMG v&aacute;lido.
		* @param fullImageName La direcci&oacute;n del archivo IMG.
		* @see getFormatType()
		*/
		Format(const char* fullImageName);

		/**
		* Destructor de la clase Format.
		*/
		~Format(void);

		/**
		* Carga la cabecera del formato IMG. Este m&eacute;todo debe ser llamado antes del m&eacute;todo loadImageData(void).
		*/
		void loadHeaderData(void);

		/**
		* Carga los datos de la imagen del formato IMG. Este m&eacute;todo debe ser llamado despu&eacute;s 
		* del m&eacute;todo loadHeaderData(void).
		*/
		void loadImageData(void);

		/**
		* Lee un byte del vector de los bytes del fichero IMG.
		* @param byteNum El n&uacute;mero del byte, la posici&oacute;n del byte que se desea leer.
		* @return El valor del byte.
		*/
		short read1Byte(int byteNum);

		/**
		* Lee 2 bytes del vector de los bytes del fichero IMG.
		* @param byteInferior El n&uacute;mero del byte, la posici&oacute;n del byte desde donde se empezar&aacute;n a leer 2 bytes.
		* @return El valor de la lectura de 2 bytes.
		*/
		double read2Bytes(int byteInferior);

		/**
		* Lee 4 bytes del vector de los bytes del fichero IMG.
		* @param byteInferior El n&uacute;mero del byte, la posici&oacute;n del byte desde donde se empezar&aacute;n a leer 4 bytes.
		* @return El valor de la lectura de 4 bytes.
		*/
		double read4Bytes(int byteInferior);

		/**
		* Lee los bytes desde inf hasta superior del vector de los bytes del fichero IMG.
		* @param inf El n&uacute;mero del byte, la posici&oacute;n del byte desde donde se empezar&aacute; a leer.
		* @param superior El n&uacute;mero del byte, la posici&oacute;n del byte hasta donde se leer&aacute;.
		* @return El texto de la lectura.
		*/
		string getString(int inf, int superior);
	};
}

#endif // Format_H
