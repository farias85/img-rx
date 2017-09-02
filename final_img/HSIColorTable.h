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

#ifndef HSICOLORTABLE_H
#define HSICOLORTABLE_H

namespace img
{
	/**
	* Define los valores de la tabla de colores para el formato HSI
	*/
	class HSIColorTable
	{
	public:
		/**
		* Constructor de la clase.
		*/
		HSIColorTable();

		/**
		* Destructor de la clase.
		*/
		~HSIColorTable();

	public:
		/**
		* Los valores estimados de la curva de mat&iacute;z.
		*/
		double *hCurve;

		/**
		* Los valores estimados de la curva intensidad.
		*/
		double *iCurve;

		/**
		* Los valores estimados de la matriz de saturaci&oacute;n respecto a los valores de intensidad y mat&iacute;z.
		*/
		double *sMatrix;
	};
}

#endif // HSICOLORTABLE_H