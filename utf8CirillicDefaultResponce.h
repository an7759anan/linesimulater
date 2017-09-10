#ifndef __UTF8CIRILLICDEFAULTRESPONCE_H__
#define __UTF8CIRILLICDEFAULTRESPONCE_H__

#include <avr/pgmspace.h>

char defaultResponce0[] PROGMEM =\
u8"HTTP/1.1 200 OK\r\n\
Content-Type: text/html; charset=utf-8\r\n\
Transfer-Encoding: chunked\r\n\
Connection: Keep-Alive\r\n\r\n";

char defaultResponce1[] PROGMEM =\
u8"<!DOCTYPE html>\
<html>\
<body>\
<form action='/'>\
<table>\
<tr><td>Длина линии:</td><td><input type='text' name='line_length' size='3' required='required' pattern='(([12]+0|[12]*[1-9])000)|(([12]+0|[12]*[1-9])*500)|0' title='длина должна быть от 0 до 20000 с шагом 500 метров' value='%u'></td><td>метров</td></tr>\
<tr><td>Обрыв:</td><td><input type='checkbox' name='breakage' value='true' %s></td></tr>\
<tr><td>Импенданс:</td>\r\n";

char defaultResponce2[] PROGMEM =\
u8"<td>\
  <select name='impendance'>\
    <option value='65535' %s>номинал</option>\
    <option value='0' %s>0</option>\
    <option value='50' %s>50</option>\
    <option value='100' %s>100</option>\
    <option value='300' %s>300</option>\
  </select>\
</td><td>Ом</td>\
</tr>\
<tr><td>Затухание:</td>\
<td>\
  <select name='attenuation'>\
    <option value='65535' %s>номинал</option>\
    <option value='10' %s>10</option>\
    <option value='40' %s>40</option>\
  </select>\
</td><td>дБ</td>\
</tr>\r\n";

char defaultResponce3[] PROGMEM =\
u8"<tr><td>Сопростивление изоляции:</td>\
<td>\
  <select name='resistance'>\
    <option value='65535' %s>номинал</option>\
    <option value='0' %s>0</option>\
    <option value='100' %s>100</option>\
    <option value='20000' %s>20000</option>\
  </select>\
</td><td>Ом</td>\
</tr>\
  </table>\
  <input type='submit'>\
</form>\
</body>\
</html>\r\n";

#endif
