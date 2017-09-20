#ifndef __UTF8CIRILLIC_H__
#define __UTF8CIRILLIC_H__

#include <avr/pgmspace.h>

char fromILtoFA_SPS_configure_start_stop_ok[] PROGMEM =\
u8"<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\
<root>\
<result action=\"%s\" state=\"1\">\
<parameters>\
<il_id>%i</il_id>\
<exp_id>%i</exp_id>\
</parameters>\
<message>Команда получена, выполняется ...</message>\
</result>\
</root>";

char fromILtoFA_SPS_configure_start_stop_error[] PROGMEM =\
u8"<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\
<root>\
<result action=\"%s\" state=\"0\">\
<parameters>\
<il_id>%i</il_id>\
<exp_id>%i</exp_id>\
</parameters>\
<message>Неверная команда: либо в текущем состоянии эта команда недопустима, либо содержит некорректные параметры</message>\
<data>Состояние КИЛ=%i, il_id=%i, exp_id=%i</data>\
</result>\
</root>";

char fromILtoFA_SPS_bad_XML_format[] PROGMEM = u8"Bad XML format\n";

char fromILtoFA_SPS_getState[] PROGMEM =\
u8"<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\
<root>\
    <result action=\"getState\" state=\"%i\">\
        <parameters>\
            <il_id>%i</il_id>\
            <exp_id>%i</exp_id>\
            <data>\
                <time>\
                    <elapsed_time>%u</elapsed_time>\
                    <time_il>%lu</time_il>\
                </time>\
                <metrics>\
                    <step>%u</step>\
                </metrics>\
            </data>\
        </parameters>\
        <message>В соответствии с кодом состояния</message>\
    </result>\
</root>";

char fromILtoFA_SPS_settings[] PROGMEM =\
u8"<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\
<root>\
    <result action=\"settings\" state=\"1\">\
        <parameters>\
            <ip>%hu.%hu.%hu.%hu</ip>\
            <ipport>%u</ipport>\
            <netmask>%hu.%hu.%hu.%hu</netmask>\
            <logger>%hu.%hu.%hu.%hu</logger>\
            <loggerport>%u</loggerport>\
            <ntp>%hu.%hu.%hu.%hu</ntp>\
        </parameters>\
        <message>Настройки контроллера ИЛ изменены</message>\
    </result>\
</root>";


#endif
