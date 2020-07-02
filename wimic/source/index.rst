.. _home:

..  raw:: html

    <h1 style="text-align:center;">Bienvenidos a la documentación de WiMic!</h1>

..  raw:: html

    <table>
    <tr>
    <td width="100%">

----------

|
|

Info Gral.
----------

Con la aplicación de WiMic puedes usar varios teléfonos o equipos móviles al mismo tiempo como micrófono inalámbrico remoto para video conferencias grupales, puedes usarlo también como micrófono remoto para megáfono ó usarlo para integrarlo con dispositvos IoT como el Raspberry Pi ó Arduinos.

WiMic se distribuye libremente y no tiene ni un costo, la donación es opcional. El uso de la aplicación en ambientes comerciales está permitido previa notificación por escrito de su uso al autor del proyecto, enviando un email a htakey@gmail.com con la referencia: "Uso de WiMic en ambiente comercial".

|
|

Motivación
----------

WiMic se desarrolló y se creó como respuesta temprana al impacto que causó la pandemia "COVID-19" a principios del 2020. El sistema está enfocado para facilitar y mejorar la experiencia de interacción con la comunicación en el área de la pedagogía escolar y la docencia en la educación superior con los alumnos y estudiantes.

Gracias a la flexibilidad y acceso al código fuente publicado de `WiMic <https://github.com/hiro2233/wimic/>`__, es posible utilizarlo en otras áreas.

|
|

Instalación y puesta en marcha
------------------------------

La aplicación funciona con el servidor de WiMic, solo debes especificar la dirección ip del servidor y conectarte, es fácil y sencillo.

Por el momento el servidor está disponible para los sistemas operativos Windows y distribuciones Linux.

El uso que se le puede dar a WiMic son numerosas, en ésta documentación explicaremos dos modos o maneras básicas de poder usarlo, uno es usándolo como un simple megáfono remoto grupal y la otra es usándolo como "micrófono virtual" para una video conferencia de clases virtuales, así poder explicar estando alejado de la PC, necesitaremos un driver de un tercero para habilitarlo, la prueba lo realizaremos con Jitsi, la configuración funciona para cualquier otra aplicaciones que use el micrófono y los altavoces de la PC, ya que WiMic se comunica a nivel de hardware.

|

[Windows]
=========

    **A)**  Descarga e instala el driver de "Virtual Audio Cable" desde éste link:

        * https://www.vb-audio.com/Cable/

        o puedes descargarlo directamente con éste link:

        * `Descargar Driver <https://download.vb-audio.com/Download_CABLE/VBCABLE_Driver_Pack43.zip/>`__

        Descomprime el archivo y extrae el contenido en cualquier carpeta de tu preferencia, puedes usar aplicaciones como el `7zip <https://www.7-zip.org/>`__ , el `Peazip <https://www.peazip.org/>`__ que se distribuyen libremente, ó WinRar.


        Dentro de la carpeta ubicas el archivo VBCABLE_Setup_x64.exe y VBCABLE_Setup.exe, la diferencia de estos dos instaladores, es que una es de 64 bits y la otra es de 32 bits, si no sabes que version de Windows tienes, prueba primero con la de 64 bits y luego con la de 32 bits. Para instalarlo debes ejecutarlo con usuario administrador, de lo contrario el driver no se instalará, para ello haz click derecho sobre el instalador y luego click en "Ejecutar como Administrador", aceptas las advertencias de administración y luego click en el botón "install driver".

        .. image:: ../../images/wind_admin_vbcable.png
            :align: center

        Luego de instalar el driver, éste aparecerá en la lista de dispositivos de salida de audio en WiMic.

|

    **B)** Descarga el servidor WiMic desde éste botón:

|

    .. image:: https://img.shields.io/github/downloads-pre/hiro2233/wimic/v1.0.2-pre-win32/total?label=Download%20WiMic&style=for-the-badge
        :alt: GitHub Pre-Releases
        :target: https://github.com/hiro2233/wimic/releases/download/v1.0.2-pre-win32/wimic.tar.gz
        :align: left

|
|

   *Nota: Si deseas verificar el checksum del archivo comprimido, éste se encuentra publicado en la sección de Release del repositorio.*

|

    **C)** Descomprime el archivo y extrae el contenido en cualquier carpeta de tu preferencia. Luego de descomprimir tendrás 3 archivos, wimic.exe, msys-2.0.dll y wimic.conf.

|

    .. image:: ../../images/wimic_archives_win.png
        :align: center

|

    **D.1)** Ejecuta wimic.exe y luego:

         **1)** selecciona "CABLE Input (VB-Audio Virtual..."

         **2)** click en el botón "select" y en la etiqueta "Device selected:" se mostrará el nombre seleccionado de la lista.

         **3)** click en el botón "start" y esperas unos 3 a 5 segundos aproximadamente hasta que el led rojo de la etiqueta "Server:" cambie a color verde.

    .. image:: ../../images/wimic_main_app.png
       :align: center

|
|

    ..

         **4)** el led rojo de la etiqueta "Server:" cambia a color verde.

         **5)** tomas nota de la dirección IP que se muestra en la etiqueta "IP: x.x.x.x", ésta dirección puede variar según la configuración que tengas en tu red, en nuestro caso nos muestra la dirección 192.168.1.16, ésta dirección IP es la que vas a necesitar para poder conectar la aplicación móvil WiMic con el servidor.


    .. image:: ../../images/wimic_main_app_started.png
        :align: center

|
|

    ..

       **6)** Toca la cruz para agregar y configurar un nuevo servidor.

|
|


    .. image:: ../../images/wimic_android_6.png
       :scale: 50 %
       :align: center

|

    ..

       **7)** Escribes la dirección IP del servidor.

       **8)** Toca añadir para completar y guardar la configuración.


|
|


    .. image:: ../../images/wimic_android_7.png
       :scale: 50 %
       :align: center

|

    ..

      **9)** Toca la caja para conectarte.

|
|


    .. image:: ../../images/wimic_android_9.png
       :scale: 50 %
       :align: center

    ..

|

      **10)** Cuando intentes conectarte a un nuevo servidor te aparecerá éste aviso una sola vez luego de aceptarla. Este certificado es generado por el servidor automáticamente en cada instalación nueva que realices y la almacena en la carpeta SSL donde se ejecutó el servidor, no aceptes certificados si no estás seguro de su procedencia. Toma en cuenta que si borras la carpeta SSL y ejecutas nuevamente el servidor, éste automáticamente creará un nuevo certificado y volverás a ver el aviso cuando intentes conectarte.
      
      Toca "PERMITIR" para completar la conexión.


|
|


    .. image:: ../../images/wimic_android_10.png
       :scale: 50 %
       :align: center

|

   ..

      Cuando la aplicación se conecta con el servidor verás la siguiente pantalla, y ya puedes empezar a hablar para transmitir voz a la aplicación de video conferencia que estés usando. En esta documentación usaremos Jitsi para poder probarlo.



|
|


    .. image:: ../../images/wimic_android_11.png
       :scale: 50 %
       :align: center





Política de Uso y Garantías
---------------------------

WiMic está desarrollado y publicado bajo licencias Open Source y Software Libre, como la Licencia General Pública (GPLv3) principalmente.

Este programa se distribuye con la esperanza de que sea útil, y SIN NINGUNA GARANTÍA. Vea la Licencia Pública General de GNU para más detalles, https://www.gnu.org/licenses



.. _home:

..  raw:: html

    <table>
    <tr>
    <td width="27%">

------

Links:
======


    `Privacy Policy <../../wimic/docs/privacy/>`__


.. toctree::
   :hidden:

   Privacy Policy <https://hiro2233.github.io/wimic/wimic/docs/privacy/>
