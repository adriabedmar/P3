PAV - P3: detección de pitch
============================

Esta práctica se distribuye a través del repositorio GitHub [Práctica 3](https://github.com/albino-pav/P3).
Siga las instrucciones de la [Práctica 2](https://github.com/albino-pav/P2) para realizar un `fork` de la
misma y distribuir copias locales (*clones*) del mismo a los distintos integrantes del grupo de prácticas.

Recuerde realizar el *pull request* al repositorio original una vez completada la práctica.

Ejercicios básicos
------------------

- Complete el código de los ficheros necesarios para realizar la detección de pitch usando el programa
  `get_pitch`.

   * Complete el cálculo de la autocorrelación e inserte a continuación el código correspondiente.
    
    ```cpp
    for (unsigned int l = 0; l < r.size(); ++l) {
        r[l] = 0;  		
        for (unsigned int n = 0; n < x.size()-l; ++n) {
          r[l] += x[n] * x[l+n];
        }
        r[l] = r[l] / x.size();
        }
    ```

   * Inserte una gŕafica donde, en un *subplot*, se vea con claridad la señal temporal de un segmento de
     unos 30 ms de un fonema sonoro y su periodo de pitch; y, en otro *subplot*, se vea con claridad la
	   autocorrelación de la señal y la posición del primer máximo secundario.

     ![Autocorrelación trama de audio de 30ms](./img/autocorrelation_30ms.png)

     La autocorrelación para esta trama de voz de 30ms tiene un pitch alrededor de 130Hz.

	 NOTA: es más que probable que tenga que usar Python, Octave/MATLAB u otro programa semejante para
	 hacerlo. Se valorará la utilización de la librería matplotlib de Python.

   * Determine el mejor candidato para el periodo de pitch localizando el primer máximo secundario de la
     autocorrelación. Inserte a continuación el código correspondiente.

      ```cpp
      vector<float>::const_iterator iR = r.begin()+npitch_min, iRMax = iR;
      while (*iR > 0)
      {
        iR++;
      }
      iRMax = iR;

      while(iR != r.end()){
        if(*iR >= *iRMax){
          iRMax = iR;
        } 
        iR++;
      }

      unsigned int lag = iRMax - r.begin();
      ```

   * Implemente la regla de decisión sonoro o sordo e inserte el código correspondiente.
    ```cpp
      if (pot > -48.9F && (r1norm > 0.93F || rmaxnorm > 0.45F)) return false;
      else return true;
      ```

- Una vez completados los puntos anteriores, dispondrá de una primera versión del detector de pitch. El 
  resto del trabajo consiste, básicamente, en obtener las mejores prestaciones posibles con él.

  * Utilice el programa `wavesurfer` para analizar las condiciones apropiadas para determinar si un
    segmento es sonoro o sordo. 
	
	  - Inserte una gráfica con la detección de pitch incorporada a `wavesurfer` y, junto a ella, los 
	    principales candidatos para determinar la sonoridad de la voz: el nivel de potencia de la señal
		(r[0]), la autocorrelación normalizada de uno (r1norm = r[1] / r[0]) y el valor de la
		autocorrelación en su máximo secundario (rmaxnorm = r[lag] / r[0]).

		  Puede considerar, también, la conveniencia de usar la tasa de cruces por cero.

	    Recuerde configurar los paneles de datos para que el desplazamiento de ventana sea el adecuado, que
		en esta práctica es de 15 ms.

      ![Parametros detección sonoridad](./img/audio_pot_r1_rmax.png)

      Como se puede ver en la imagen el valor rmaxnorm marca los tramos sonoros con un valor superior a unos 0.5,
      la potencia también está por encima de los -40dB y la r1norm por encima de 0.8.

      - Use el detector de pitch implementado en el programa `wavesurfer` en una señal de prueba y compare
	    su resultado con el obtenido por la mejor versión de su propio sistema.  Inserte una gráfica
		ilustrativa del resultado de ambos detectores.
      ![Parametros detección sonoridad optimizado para rl002.wav](./img/opt_param_rl002.png)

      Con los parametros por defecto que se pueden ver en la figura anterior se obtiene el siguiente resultado:
      ![Resultado optimizado para rl002.wav](./img/opt_pitch_rl002.png)
      
      Si vemos se evalua con el programa `pitch_evaluate` se ve como se han reducido los errores:
      ![Evaluación resultado optimizado para rl002.wav](./img/optimized_rl002.png)

  
  * Optimice los parámetros de su sistema de detección de pitch e inserte una tabla con las tasas de error
    y el *score* TOTAL proporcionados por `pitch_evaluate` en la evaluación de la base de datos 
	`pitch_db/train`..
    Se ha optimizado con la buscqueda iterativa mediante la CLI.
    ![Centre clipping optimization](./img/optimization_train.png)
    ![Centre clipping optimization parameters](./img/opt_train_param.png)

   * Inserte una gráfica en la que se vea con claridad el resultado de su detector de pitch junto al del
     detector de Wavesurfer. Aunque puede usarse Wavesurfer para obtener la representación, se valorará
	 el uso de alternativas de mayor calidad (particularmente Python).
   ![Copmarison calculated pitch and reference](./img/rl002_pitch_graph.png)
   

Ejercicios de ampliación
------------------------

- Usando la librería `docopt_cpp`, modifique el fichero `get_pitch.cpp` para incorporar los parámetros del
  detector a los argumentos de la línea de comandos.
  
  Esta técnica le resultará especialmente útil para optimizar los parámetros del detector. Recuerde que
  una parte importante de la evaluación recaerá en el resultado obtenido en la detección de pitch en la
  base de datos.

  * Inserte un *pantallazo* en el que se vea el mensaje de ayuda del programa y un ejemplo de utilización
    con los argumentos añadidos.
    ![Ayuda linea de comandos](./img/docopt_h.png)
    ![Ejemplo linea de comandos](./img/docopt_example.png)

- Implemente las técnicas que considere oportunas para optimizar las prestaciones del sistema de detección
  de pitch.

  Entre las posibles mejoras, puede escoger una o más de las siguientes:

  * Técnicas de preprocesado: filtrado paso bajo, *center clipping*, etc.
        ```cpp
        //Iterate through the whole audio to get the minimum and maximum value to clip the file relative to their values
        vector<float>::iterator iX;
        for(iX = x.begin(); iX < x.end(); iX++){    
          if(*iX > max){
            max = *iX;
          }   
          if(*iX < min){      
            min = *iX;
          }  
        }

        if(-1*(min) > max){  
          range = -1*(min);  
        }else{
          range = max;
        } 
        //Set the clipping range relative to the maximum, with a value of clip form 0 to 1. This value is 0 by default and can be set through the CLI
        clip = range * clip;

        //Eliminate all values lower than the threshold and substract the threshold to the higher values
        for(iX = x.begin(); iX<x.end(); iX++){
          if(*iX<0){
            abs = -1*(*iX);
          }
          else{
            abs = *iX;
          }
          if(abs<clip){      
            *iX = 0;
          }    
          else{
            if(*iX > 0)        
              *iX = *iX - clip;
            else        
              *iX = *iX + clip;    
          } 
        }
        ```
    Con la implementación del 'center clipping' se obtiene un resultado optimizado para el dataset de `train` del 90.96%
    ![Centre clipping optimization](./img/optimization_train.png)
    Se ha conseguido con una busqueda iterativa que usa la CLI del apartado anterior. El bucle de busca se encuentra en el fichero `scripts/optimization.sh`
  * Técnicas de postprocesado: filtro de mediana, *dynamic time warping*, etc.
  * Métodos alternativos a la autocorrelación: procesado cepstral, *average magnitude difference function*
    (AMDF), etc.
  * Optimización **demostrable** de los parámetros que gobiernan el detector, en concreto, de los que
    gobiernan la decisión sonoro/sordo.
  * Cualquier otra técnica que se le pueda ocurrir o encuentre en la literatura.

  Encontrará más información acerca de estas técnicas en las [Transparencias del Curso](https://atenea.upc.edu/pluginfile.php/2908770/mod_resource/content/3/2b_PS%20Techniques.pdf)
  y en [Spoken Language Processing](https://discovery.upc.edu/iii/encore/record/C__Rb1233593?lang=cat).
  También encontrará más información en los anexos del enunciado de esta práctica.

  Incluya, a continuación, una explicación de las técnicas incorporadas al detector. Se valorará la
  inclusión de gráficas, tablas, código o cualquier otra cosa que ayude a comprender el trabajo realizado.

  También se valorará la realización de un estudio de los parámetros involucrados. Por ejemplo, si se opta
  por implementar el filtro de mediana, se valorará el análisis de los resultados obtenidos en función de
  la longitud del filtro.
   

Evaluación *ciega* del detector
-------------------------------

Antes de realizar el *pull request* debe asegurarse de que su repositorio contiene los ficheros necesarios
para compilar los programas correctamente ejecutando `make release`.

Con los ejecutables construidos de esta manera, los profesores de la asignatura procederán a evaluar el
detector con la parte de test de la base de datos (desconocida para los alumnos). Una parte importante de
la nota de la práctica recaerá en el resultado de esta evaluación.
