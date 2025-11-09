<h1>ESP32_Arcade</h1>
<img width="50%" height="50%" alt="01" src="https://github.com/user-attachments/assets/de96fb38-01c5-4426-aec2-3c4426ddab72"><img width="50%" height="50%" alt="02" src="https://github.com/user-attachments/assets/05a892e8-6db8-4fbd-9bed-10b650cca896">
<br>
Tiny capsule toy of Japanese arcade game cabinet that showing intro movies.

Can play many movies you want with SD card storage!<br><br>

<img width="50%" height="50%" alt="original" src="https://github.com/user-attachments/assets/266b8bd3-39b4-4b6a-b4ac-f61e7caf56d2">
<br>
Original capsule toy

<h2>Features</h2>
Playing 15FPS movie file with sounds<br>
Auto looping<br>
Unlimited number of movie file with SD card storage<br>
Selection with a tact switch<br>
Volume setting by config.cfg<br>

<h2>Hardware components</h2>
<a href="https://ko.aliexpress.com/item/1005006524672028.html?spm=a2g0o.order_list.order_list_main.377.3bb51802MgtiNS&gatewayAdapt=glo2kor" target="_blank">ESP32-S3-Zero</a><br>
<a href="https://ko.aliexpress.com/item/1005008625277253.html?spm=a2g0o.order_list.order_list_main.192.3bb51802MgtiNS&gatewayAdapt=glo2kor" target="_blank">ST7789 SPI TFT LCD (240x240, 1.54inch)</a><br>
<a href="https://ko.aliexpress.com/item/1005006823278901.html?spm=a2g0o.productlist.main.2.5d98795cx1O3qf&algo_pvid=e03122a9-dd63-4541-bc10-53d38bfca166&algo_exp_id=e03122a9-dd63-4541-bc10-53d38bfca166-1&pdp_ext_f=%7B%22order%22%3A%22349%22%2C%22eval%22%3A%221%22%2C%22fromPage%22%3A%22search%22%7D&pdp_npi=6%40dis%21KRW%211485%211421%21%21%217.18%216.87%21%40213ba8cc17620700599617966ef45b%2112000038418438949%21sea%21KR%21703843503%21X%211%210%21n_tag%3A-29919%3Bd%3A793254b4%3Bm03_new_user%3A-29895%3BpisId%3A5000000191942983&curPageLogUid=d8GkQFkKoM6h&utparam-url=scene%3Asearch%7Cquery_from%3A%7Cx_object_id%3A1005006823278901%7C_p_origin_prod%3A" target="_blank">SD card module</a><br>
<a href="https://ko.aliexpress.com/item/1005009482707515.html?spm=a2g0o.productlist.main.1.33433ce0o1JpXF&algo_pvid=60d22ead-617d-40c6-87ad-70355fb03f4d&algo_exp_id=60d22ead-617d-40c6-87ad-70355fb03f4d-0&pdp_ext_f=%7B%22order%22%3A%22227%22%2C%22eval%22%3A%221%22%2C%22fromPage%22%3A%22search%22%7D&pdp_npi=6%40dis%21KRW%211858%211803%21%21%218.98%218.71%21%402140f54217620700192688762eea14%2112000049240097021%21sea%21KR%21703843503%21X%211%210%21n_tag%3A-29919%3Bd%3A793254b4%3Bm03_new_user%3A-29895%3BpisId%3A5000000191975020&curPageLogUid=Mu3uunN1bOn4&utparam-url=scene%3Asearch%7Cquery_from%3A%7Cx_object_id%3A1005009482707515%7C_p_origin_prod%3A" target="_blank">MAX98357A DAC+Amp</a><br>
<a href="https://ko.aliexpress.com/item/1005006049127824.html?spm=a2g0o.productlist.main.9.7a9a57e4Ay63OI&algo_pvid=c113cb85-082b-4113-9a81-c52e0ea0fccd&algo_exp_id=c113cb85-082b-4113-9a81-c52e0ea0fccd-8&pdp_ext_f=%7B%22order%22%3A%22129%22%2C%22eval%22%3A%221%22%2C%22fromPage%22%3A%22search%22%7D&pdp_npi=6%40dis%21KRW%211390%211390%21%21%216.72%216.72%21%40213ba8cc17620701227788759ef42b%2112000035489554249%21sea%21KR%21703843503%21X%211%210%21n_tag%3A-29919%3Bd%3A793254b4%3Bm03_new_user%3A-29895&curPageLogUid=A7WTzcdEuYTc&utparam-url=scene%3Asearch%7Cquery_from%3A%7Cx_object_id%3A1005006049127824%7C_p_origin_prod%3A" target="_blank">Tact switch</a><br>
<a href="https://ko.aliexpress.com/item/1005008070859020.html?spm=a2g0o.order_list.order_list_main.11.3bb51802MgtiNS&gatewayAdapt=glo2kor" target="_blank">20mm speaker</a><br>

<h2>Software libraries</h2>
lovyanGFX<br>
SD.h<br>
SPI.h<br>
vector<br>
algorithm<br>

<h2>Connection</h2>
<b>* ESP32-S3-Zero --> TFT LCD<br></b>
&nbsp;&nbsp;&nbsp;cfg.pin_sclk = 13; // SCL<br>
&nbsp;&nbsp;&nbsp;cfg.pin_mosi = 12; // SCA<br>
&nbsp;&nbsp;&nbsp;cfg.pin_rst = 11; // RES<br>
&nbsp;&nbsp;&nbsp;cfg.pin_dc = 10;<br>
&nbsp;&nbsp;&nbsp;cfg.spi_mode = 3;<br>
&nbsp;&nbsp;&nbsp;BLK : not necessary to connect<br>
&nbsp;&nbsp;&nbsp;miso, cs, busy : set to -1<br>
<br>
<b>* ESP32-S3-Zero --> MAX98357A<br></b>
&nbsp;&nbsp;&nbsp;LRC = 9<br>
&nbsp;&nbsp;&nbsp;BCLK = 8<br>
&nbsp;&nbsp;&nbsp;DIN = 7<br>
&nbsp;&nbsp;&nbsp;GAIN -> GND<br>
&nbsp;&nbsp;&nbsp;SD -> VIN<br>
<br>
<b>* ESP32-S3-Zero --> SD card module<br></b>
&nbsp;&nbsp;&nbsp;SD_CS = 4<br>
&nbsp;&nbsp;&nbsp;SD_MOSI = 3<br>
&nbsp;&nbsp;&nbsp;SD_CLK = 2<br>
&nbsp;&nbsp;&nbsp;SD_MISO = 1<br>
   
<h2>Encoding</h2>
<b>* Video<br></b>
ffmpeg -i input.mp4 -vf "crop=960:672:160:24,scale=240:168,setpts=0.675*PTS,fps=15" -c:v rawvideo -pix_fmt rgb565 -f rawvideo output.raw<br>
&nbsp;&nbsp;&nbsp;crop=960:672:160:24 -> Video crop<br>
&nbsp;&nbsp;&nbsp;scale=240:168 -> Video resize<br>
&nbsp;&nbsp;&nbsp;setpts=0.675*PTS,fps=15 -> Optimize for ESP32 playback performance<br>
<br>
<b>* Audio<br></b>
ffmpeg -i source.mp4 -vn -acodec pcm_s16le -ar 22050 -ac 1 -t 10.0 -filter:a "volume=2.0" output.wav<br>




