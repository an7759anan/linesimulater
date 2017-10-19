#ifndef __UTF8CIRILLIC_H__
#define __UTF8CIRILLIC_H__

#include <avr/pgmspace.h>

char fromILtoFA_SPS_configure_start_stop_ok[] PROGMEM =\
u8"<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\
<root>\
<result action=\"%s\" state=\"1\">\
<parameters>\
<il_id>%i</il_id>\
<exp_id>%lu</exp_id>\
</parameters>\
<message>Команда получена, выполняется ...</message>\
</result>\
</root>";

char fromILtoFA_SPS_start_stop_absent_params[] PROGMEM =\
u8"<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\
<root>\
<result action=\"%s\" state=\"0\">\
<message>Отсутствуют необходимые параметры</message>\
</result>\
</root>";

char fromILtoFA_SPS_configure_start_stop_error[] PROGMEM =\
u8"<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\
<root>\
<result action=\"%s\" state=\"0\">\
<parameters>\
<il_id>%i</il_id>\
<exp_id>%lu</exp_id>\
</parameters>\
<message>Неверная команда: либо в текущем состоянии эта команда недопустима, либо содержит некорректные параметры</message>\
<data>Состояние КИЛ=%i, il_id=%i, exp_id=%lu</data>\
</result>\
</root>";


char fromILtoFA_SPS_start_bad_start_time[] PROGMEM =\
u8"<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\
<root>\
<result action=\"start\" state=\"0\">\
<message>Ошибочные параметры (время старта из прошлого)</message>\
</result>\
</root>";

char fromILtoFA_SPS_uncknown_command[] PROGMEM =\
u8"<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\
<root>\
<result action=\"unknown\" state=\"0\">\
<message>Некорректный запрос (отсутствующий в списке поддерживаемых запросов / action)</message>\
</result>\
</root>";

char fromILtoFA_SPS_bad_XML_format[] PROGMEM =\
u8"<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\
<root>\
<result action=\"unknown\" state=\"0\">\
<message>Неправильный XML формат</message>\
</result>\
</root>";

char fromILtoFA_SPS_getState[] PROGMEM =\
u8"<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\
<root>\
    <result action=\"getState\" state=\"%i\">\
        <parameters>\
            <il_id>%i</il_id>\
            <exp_id>%lu</exp_id>\
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
