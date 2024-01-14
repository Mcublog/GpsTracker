
/**
 * @file config.c
 * @author Viacheslav (viacheslav@mcublog.ru)
 * @brief
 * @version 0.1
 * @date 2023-02-25
 *
 * @copyright Viacheslav mcublog (c) 2023
 *
 */
#include "app/config/config.h"
#include "app/utils/crc32.h"
#include "platforms/hw/config/common.h"

#include "stm32f4xx_hal.h"
//>>---------------------- Log control
#define LOG_MODULE_NAME config
#if defined(NDEBUG)
#define LOG_MODULE_LEVEL (0)
#else
#define LOG_MODULE_LEVEL (3)
#endif
#include "app/debug/log_libs.h"
//<<----------------------
//>>---------------------- Local declaration
#define CONFIG_CRC_IDX              ((sizeof(config_t) >> 2) - 1) // индекс crc32
#define CONFIG_DATA_SECTOR_NUM      (FLASH_SECTOR_11)
#define CONFIG_DATA_FLASH_SECTOR    (16384>>4)
#define CONFIG_DATA_MAX_ADR         (CONFIG_DATA_FLASH_SECTOR/(sizeof(config_t)>>2))

const uint32_t *FLASH_SECTOR = (const uint32_t *)CONFIG_FLASH_SECTOR;
//<<----------------------

//>>---------------------- Local defenition
/*-----------------------------------------------------------
/Производит стернаие блока данных содержащий конфиг
/принимает:
/вовращает:
-----------------------------------------------------------*/
void _config_erase_sector(void)
{
    HAL_FLASH_Unlock();
    FLASH_Erase_Sector(CONFIG_DATA_SECTOR_NUM, FLASH_VOLTAGE_RANGE_3);
    HAL_FLASH_Lock();
}

/*-----------------------------------------------------------
/Производит запись 32b слова во флешь
/принимает: полный адрес, слово для записи
/вовращает:
-----------------------------------------------------------*/
void _config_write_word32(uint32_t full_adr, uint32_t word)
{
    HAL_StatusTypeDef s = HAL_ERROR;
    HAL_FLASH_Unlock();

    while (s == HAL_ERROR)
    {
        __HAL_FLASH_DATA_CACHE_DISABLE();
        __HAL_FLASH_INSTRUCTION_CACHE_DISABLE();

        __HAL_FLASH_DATA_CACHE_RESET();
        __HAL_FLASH_INSTRUCTION_CACHE_RESET();

        __HAL_FLASH_INSTRUCTION_CACHE_ENABLE();
        __HAL_FLASH_DATA_CACHE_ENABLE();
        s = HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, full_adr, word);
    }
    HAL_FLASH_Lock();
}

/*-----------------------------------------------------------
/Получить полный адрес
принимает: значение относительного адреса (считает в кол-ве
config_t) /вовращает: полный адрес абослютный(Начало сектора)+ смещение по масиву
-----------------------------------------------------------*/
static uint32_t _config_get_full_adr(uint32_t adr)
{
    //(adr*sizeof(config_t)>>2) -- перевожу адрес оностиельно структуры config_t в адрес
    // uint32_t
    return (uint32_t)(&FLASH_SECTOR[(adr * sizeof(config_t) >> 2)]);
}

/*-----------------------------------------------------------
/Помечаю по относительнуму adr config как старый
/принимает: значение относительного адреса (считает в кол-ве config_t)
/вовращает:
-----------------------------------------------------------*/
void _config_set_old(uint32_t adr)
{
    uint32_t Address = _config_get_full_adr(adr);
    _config_write_word32(Address, CONFIG_OLD_CONFIG);
}

/*-----------------------------------------------------------
/Производит запись конфига
/принимает: значение относительного адреса (считает в кол-ве config_t), указаетель на
конфиг /вовращает:
-----------------------------------------------------------*/
void _config_write_cfg(uint32_t adr, config_t *cfg)
{
    uint32_t i = 0;
    uint32_t Address = _config_get_full_adr(adr);
    LOG_INFO("Address: %#x", Address);
    // uint32_t crc32=0;
    uint32_t *cfgp = (uint32_t *)cfg;
    config_t *p = (config_t *)Address;
    cfgp[CONFIG_CRC_IDX] =
        xcrc32((uint8_t *)cfgp, sizeof(config_t) - CRC32_LENGHT, 0xFFFFFFFF);
    LOG_INFO("Setting CRC32 write: %#x", cfgp[CONFIG_CRC_IDX]);
    for (i = 0; i < (sizeof(config_t) >> 2); i++)
    {
        _config_write_word32(Address + (i * sizeof(uint32_t)), cfgp[i]);
    }
    LOG_INFO("Setting CRC32 Read: %#x", p->crc32);
    LOG_INFO("Setting CRC32 Cnt: %#x",
             xcrc32((uint8_t *)p, sizeof(config_t) - CRC32_LENGHT, 0xFFFFFFFF));
}

/*-----------------------------------------------------------
/Проверить свободено ли место для конфига
/принимает: значение относительного адреса (считает в кол-ве config_t)
/вовращает: 1 если место свободно и можно записать, 0 если уже записано
-----------------------------------------------------------*/
uint32_t _config_check_cfg_free(uint32_t adr)
{
    uint32_t i = 0;
    uint32_t *cfgp = (uint32_t *)_config_get_full_adr(adr);
    uint32_t check_free = 0; // счетчик пустого места (все 1)
    for (i = 0; i < sizeof(config_t) >> 2; i++)
    {
        if (cfgp[i] == 0xffffffff)
            check_free++;
    }
    if (check_free == sizeof(config_t) >> 2)
        return 1; // все единицы место свободно
    else
        return 0;
}

/*-----------------------------------------------------------
/Производит сохранение конфига
/принимает: казаетль на конфиг
/вовращает: 1 если все записалось и контрольная сумма совпала, 0 если ошибка
-----------------------------------------------------------*/
config_error_t config_save(config_t *cfg)
{
    uint32_t adr = 0;
    config_t *cfgp = (config_t *)FLASH_SECTOR; // читаю по нему config из массива
    // читать с нулевого адреса, до тех пор пока не встречу признак CONFIG_CURRENT_CONFIG
    // если адрес >0 то в предыдущую запись прописать CONFIG_OLD_CONFIG в переменную last
    cfg->last = CONFIG_CURRENT_CONFIG; // опрделяю что будут записаны актуальные настройки
    while (adr < CONFIG_DATA_MAX_ADR)
    {
        if (cfgp[adr].last == CONFIG_CURRENT_CONFIG)
        {
            if (_config_check_cfg_free(adr)) // проверяю пустой ли место
            {
                if (adr != 0) // если не начала флеша
                {
                    _config_set_old(adr - 1); // то помечаем предыдущй конфиг как старый
                }
                else
                    _config_set_old(adr); // иначе помечаю текущий
            }
            else // есть конфиг на данном месте
            {
                _config_set_old(adr); // иначе помечаю текущий
            }
            if (cfgp[adr].last == CONFIG_OLD_CONFIG)
                adr++; // если конфиг устарел пишу в следущий адрес
            // иначе место свободно и можно писать по текущему адресу
            if (adr > CONFIG_DATA_MAX_ADR)
                break;
            else
            {
                LOG_INFO("cfg_save at adr: %d", adr);
                // записать новые настройки
                _config_write_cfg(adr, cfg);
                break;
            }
        }
        else
            adr++;
    }
    if (adr >= CONFIG_DATA_MAX_ADR) // нет свободного места, стераю весь сектор
    {
        // стереть сектор
        adr = 0;
        LOG_INFO("config erase sector");
        LOG_INFO("cfg_save at adr: %d", adr);
        _config_erase_sector();
        _config_write_cfg(adr, cfg);
    }
    return CONFIG_OK;
}
/*-----------------------------------------------------------
/Чтение последнего актуального конфига
/принимает: eказаетль на конфиг
/вовращает: 1 если конфиг актуален, 0 если не смог найти настройки
-----------------------------------------------------------*/
config_error_t config_load(config_t *cfg)
{
    uint32_t adr = 0;
    config_t *cfgp = (config_t *)FLASH_SECTOR; // читаю по нему config из массива
    // читать с нулевого адреса, до тех пор пока не встречу признак CONFIG_CURRENT_CONFIG
    // если адрес >0 то в предыдущую запись прописать CONFIG_OLD_CONFIG в переменную last
    cfg->last = CONFIG_CURRENT_CONFIG; // опрделяю что будут записаны актуальные настройки
    while (adr < CONFIG_DATA_MAX_ADR)
    {
        if (cfgp[adr].last != CONFIG_CURRENT_CONFIG)
        {
            adr++;
            continue;
        }

        if (_config_check_cfg_free(adr) == 0) // конфиг найден
        {
            LOG_DEBUG("Actual config find at adr: %d", adr);
            *cfg = cfgp[adr];
            return CONFIG_OK;
        }

        adr++;

    }
    if (adr >= CONFIG_DATA_MAX_ADR) // не нашел конфиг
    {
        LOG_INFO("Actual config not found!!!");
        return CONFIG_ERROR;
    }
    return CONFIG_OK;
}

/*-----------------------------------------------------------
/Производит стернаие блока данных содержащий конфиг т.е. удаляет все настройки
/принимает:
/вовращает:
-----------------------------------------------------------*/
config_error_t config_erase(void)
{
    _config_erase_sector();
    return CONFIG_OK;
}
