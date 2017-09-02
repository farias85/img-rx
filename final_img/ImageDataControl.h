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

#ifndef IMAGEDATACONTROL_H
#define IMAGEDATACONTROL_H

namespace img
{
	/**
	* Define la forma en que est&aacute;n estructurados los bytes que conforman la imagen en el campo de datos del fichero IMG
	*/
	class ImageDataControl
	{
	private:
		/**
		* N&uacute;mero del byte donde comienza la estructura.
		*/
		long startByte;

		/**
		* Tama&ntilde;o en bytes de la columna. Cantidad de bytes que codifican la columna.
		*/
		int columnLength;

		/**
		* Longitud de fondo. Cantidad de bytes que hay que desplazarse desde el inicio de una columna
		* para encontrar la primera posici&oacute;n de datos de la imagen.
		*/
		int zeroPadding;
	public:
		/**
		* Constructor de la clase.
		* @param startByte N&uacute;mero del byte donde comienza la estructura.
		* @param columnLength Tama&ntilde;o en bytes de la columna. Cantidad de bytes que codifican la columna.
		* @param zeroPadding Longitud de fondo. Cantidad de bytes que hay que desplazarse desde el inicio de una columna
		* para encontrar la primera posici&oacute;n de datos de la imagen.
		*/
		ImageDataControl(long startByte, int columnLength, int zeroPadding);

		/**
		* Destructor de la clase.
		*/
		~ImageDataControl();

		/**
		* N&uacute;mero del byte donde comienza la estructura.
		*/
		long getStartByte() { return this->startByte; }

		/**
		* Tama&ntilde;o en bytes de la columna. Cantidad de bytes que codifican la columna.
		*/
		int getColumnLength() { return this->columnLength; }

		/**
		* Longitud de fondo. Cantidad de bytes que hay que desplazarse desde el inicio de una columna
		* para encontrar la primera posici&oacute;n de datos de la imagen.
		*/
		int getZeroPadding() { return this->zeroPadding; }
	};
}
#endif // IMAGEDATACONTROL_H
