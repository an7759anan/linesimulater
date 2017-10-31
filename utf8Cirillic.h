#ifndef __UTF8CIRILLIC_H__
#define __UTF8CIRILLIC_H__

#include <avr/pgmspace.h>

char commandStartOk[] PROGMEM = u8"Команда старта эксперимента получена и выполняется";
char commandStopOk[] PROGMEM = u8"Команда остановки эксперимента получена, эксперимент прекращен";
char commandConfigureOk[] PROGMEM = u8"Команда на конфигурирование оборудования получена, начато конфигурирование";

char fromILtoFA_SPS_configure_start_stop_ok[] PROGMEM =\
u8"<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\
<root>\
<result action=\"%s\" state=\"1\">\
<parameters>\
<il_id>%i</il_id>\
<exp_id>%lu</exp_id>\
</parameters>\
<message>%s</message>\
</result>\
</root>";

char fromILtoFA_SPS_start_stop_absent_params[] PROGMEM =\
u8"<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\
<root>\
<result action=\"%s\" state=\"0\">\
<message>Отсутствуют необходимые параметры</message>\
</result>\
</root>";

char fromILtoFA_SPS_configure_start_stop_error_not_allowed[] PROGMEM =\
u8"<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\
<root>\
<result action=\"%s\" state=\"0\">\
<parameters>\
<il_id>%i</il_id>\
<exp_id>%lu</exp_id>\
</parameters>\
<message>Команда в текущем состоянии контроллера недопустима</message>\
<data>Состояние КИЛ=%i, il_id=%i, exp_id=%lu</data>\
</result>\
</root>";

char fromILtoFA_SPS_configure_start_stop_error_bad_params[] PROGMEM =\
u8"<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\
<root>\
<result action=\"%s\" state=\"0\">\
<parameters>\
<il_id>%i</il_id>\
<exp_id>%lu</exp_id>\
</parameters>\
<message>Команда содержит некорректные параметры</message>\
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
        <message>%s%s</message>\
    </result>\
</root>";

char state0[] PROGMEM = u8"эксперимент завершён с ошибкой";
char state1[] PROGMEM = u8"ожидает времени начала эксперимента";
char state2[] PROGMEM = u8"в процессе выполнения эксперимента";
char state3[] PROGMEM = u8"эксперимент завершён успешно";
char state4[] PROGMEM = u8"исходные данные не загружены (КИЛ не сконфигурирован)";
char state5[] PROGMEM = u8"эксперимент завершён по команде АРМ ООРЭ";
char state6[] PROGMEM = u8"контроллер искусственной линии в процессе конфигурирования";
char state7[] PROGMEM = u8"возникла ошибка конфигурирования";
char state8[] PROGMEM = u8"контроллер искусственной линии сконфигурирован, ожидает команды старта";
char state9[] PROGMEM = u8"контроллер искусственной линии находится в состоянии аварии";
char state11[] PROGMEM = u8"контроллер искусственной линии находится в процессе остановки";

char *stateStrings[] PROGMEM = {
     state0,
     state1,
     state2,
     state3,
     state4,
     state5,
     state6,
     state7,
     state8,
     state9,
     0,
     state11
};	

char stateNothing[] PROGMEM = u8"";
char state7XmlError[] PROGMEM = u8": неверный формат файла конфигурации";
char state7NtpError[] PROGMEM = u8": нет ответа от NTP сервера";

char *stateErrorStrings[] PROGMEM = {
	 stateNothing,
	 state7XmlError,
	 state7NtpError
};

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
