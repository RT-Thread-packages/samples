/*
 * File      : drv_mpu6050.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2006 - 2018, RT-Thread Development Team
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Change Logs:
 * Date           Author       Notes
 * 2017-01-08     DQL          first implementation.
 */

#include "drv_mpu6050.h"

#if 1
    #define MPUDEBUG      rt_kprintf
#else
    #define MPUDEBUG(...)
#endif

/*mpu6050 registers define*/
#define MPU_SELF_TESTX_REG      0X0D    //�Լ�Ĵ���X
#define MPU_SELF_TESTY_REG      0X0E    //�Լ�Ĵ���Y
#define MPU_SELF_TESTZ_REG      0X0F    //�Լ�Ĵ���Z
#define MPU_SELF_TESTA_REG      0X10    //�Լ�Ĵ���A
#define MPU_SAMPLE_RATE_REG     0X19    //����Ƶ�ʷ�Ƶ��
#define MPU_CFG_REG             0X1A    //���üĴ���
#define MPU_GYRO_CFG_REG        0X1B    //���������üĴ���
#define MPU_ACCEL_CFG_REG       0X1C    //���ٶȼ����üĴ���
#define MPU_MOTION_DET_REG      0X1F    //�˶���ֵⷧ���üĴ���
#define MPU_FIFO_EN_REG         0X23    //FIFOʹ�ܼĴ���
#define MPU_I2CMST_CTRL_REG     0X24    //IIC�������ƼĴ���
#define MPU_I2CSLV0_ADDR_REG    0X25    //IIC�ӻ�0������ַ�Ĵ���
#define MPU_I2CSLV0_REG         0X26    //IIC�ӻ�0���ݵ�ַ�Ĵ���
#define MPU_I2CSLV0_CTRL_REG    0X27    //IIC�ӻ�0���ƼĴ���
#define MPU_I2CSLV1_ADDR_REG    0X28    //IIC�ӻ�1������ַ�Ĵ���
#define MPU_I2CSLV1_REG         0X29    //IIC�ӻ�1���ݵ�ַ�Ĵ���
#define MPU_I2CSLV1_CTRL_REG    0X2A    //IIC�ӻ�1���ƼĴ���
#define MPU_I2CSLV2_ADDR_REG    0X2B    //IIC�ӻ�2������ַ�Ĵ���
#define MPU_I2CSLV2_REG         0X2C    //IIC�ӻ�2���ݵ�ַ�Ĵ���
#define MPU_I2CSLV2_CTRL_REG    0X2D    //IIC�ӻ�2���ƼĴ���
#define MPU_I2CSLV3_ADDR_REG    0X2E    //IIC�ӻ�3������ַ�Ĵ���
#define MPU_I2CSLV3_REG         0X2F    //IIC�ӻ�3���ݵ�ַ�Ĵ���
#define MPU_I2CSLV3_CTRL_REG    0X30    //IIC�ӻ�3���ƼĴ���
#define MPU_I2CSLV4_ADDR_REG    0X31    //IIC�ӻ�4������ַ�Ĵ���
#define MPU_I2CSLV4_REG         0X32    //IIC�ӻ�4���ݵ�ַ�Ĵ���
#define MPU_I2CSLV4_DO_REG      0X33    //IIC�ӻ�4д���ݼĴ���
#define MPU_I2CSLV4_CTRL_REG    0X34    //IIC�ӻ�4���ƼĴ���
#define MPU_I2CSLV4_DI_REG      0X35    //IIC�ӻ�4�����ݼĴ���

#define MPU_I2CMST_STA_REG      0X36    //IIC����״̬�Ĵ���
#define MPU_INTBP_CFG_REG       0X37    //�ж�/��·���üĴ���
#define MPU_INT_EN_REG          0X38    //�ж�ʹ�ܼĴ���
#define MPU_INT_STA_REG         0X3A    //�ж�״̬�Ĵ���

#define MPU_ACCEL_XOUTH_REG     0X3B    //���ٶ�ֵ,X���8λ�Ĵ���
#define MPU_ACCEL_XOUTL_REG     0X3C    //���ٶ�ֵ,X���8λ�Ĵ���
#define MPU_ACCEL_YOUTH_REG     0X3D    //���ٶ�ֵ,Y���8λ�Ĵ���
#define MPU_ACCEL_YOUTL_REG     0X3E    //���ٶ�ֵ,Y���8λ�Ĵ���
#define MPU_ACCEL_ZOUTH_REG     0X3F    //���ٶ�ֵ,Z���8λ�Ĵ���
#define MPU_ACCEL_ZOUTL_REG     0X40    //���ٶ�ֵ,Z���8λ�Ĵ���

#define MPU_TEMP_OUTH_REG       0X41    //�¶�ֵ�߰�λ�Ĵ���
#define MPU_TEMP_OUTL_REG       0X42    //�¶�ֵ��8λ�Ĵ���

#define MPU_GYRO_XOUTH_REG      0X43    //������ֵ,X���8λ�Ĵ���
#define MPU_GYRO_XOUTL_REG      0X44    //������ֵ,X���8λ�Ĵ���
#define MPU_GYRO_YOUTH_REG      0X45    //������ֵ,Y���8λ�Ĵ���
#define MPU_GYRO_YOUTL_REG      0X46    //������ֵ,Y���8λ�Ĵ���
#define MPU_GYRO_ZOUTH_REG      0X47    //������ֵ,Z���8λ�Ĵ���
#define MPU_GYRO_ZOUTL_REG      0X48    //������ֵ,Z���8λ�Ĵ���

#define MPU_I2CSLV0_DO_REG      0X63    //IIC�ӻ�0���ݼĴ���
#define MPU_I2CSLV1_DO_REG      0X64    //IIC�ӻ�1���ݼĴ���
#define MPU_I2CSLV2_DO_REG      0X65    //IIC�ӻ�2���ݼĴ���
#define MPU_I2CSLV3_DO_REG      0X66    //IIC�ӻ�3���ݼĴ���

#define MPU_I2CMST_DELAY_REG    0X67    //IIC������ʱ�����Ĵ���
#define MPU_SIGPATH_RST_REG     0X68    //�ź�ͨ����λ�Ĵ���
#define MPU_MDETECT_CTRL_REG    0X69    //�˶������ƼĴ���
#define MPU_USER_CTRL_REG       0X6A    //�û����ƼĴ���
#define MPU_PWR_MGMT1_REG       0X6B    //��Դ�����Ĵ���1
#define MPU_PWR_MGMT2_REG       0X6C    //��Դ�����Ĵ���2 
#define MPU_FIFO_CNTH_REG       0X72    //FIFO�����Ĵ����߰�λ
#define MPU_FIFO_CNTL_REG       0X73    //FIFO�����Ĵ����Ͱ�λ
#define MPU_FIFO_RW_REG         0X74    //FIFO��д�Ĵ���
#define MPU_DEVICE_ID_REG       0X75    //����ID�Ĵ���

#define MPU6050_ADDR                0X68

#define MPU6050_I2CBUS_NAME  "i2c2"     /* I2C�豸����,�����drv_i2c.cע���I2C�豸����һ�� */

static struct rt_i2c_bus_device *mpu6050_i2c_bus;    /* I2C�豸��� */

/*
METHOD == 0��ʹ��rt_i2c_transfer()�ķ�װ
METHOD == 1��ʹ��rt_i2c_master_send()����rt_i2c_master_recv()�ķ�װ
*/
#define METHOD 1

#if METHOD == 0

//дmpu6050�����Ĵ���
//reg:�Ĵ�����ַ
//data:����
//����ֵ: 0,���� / -1,�������
rt_err_t mpu6050_write_reg(rt_uint8_t reg, rt_uint8_t data)
{
    struct rt_i2c_msg msgs;
    rt_uint8_t buf[2] = {reg, data};

    msgs.addr  = MPU6050_ADDR;    /* �ӻ���ַ */
    msgs.flags = RT_I2C_WR;       /* д��־ */
    msgs.buf   = buf;             /* ��������ָ�� */
    msgs.len   = 2;

    if (rt_i2c_transfer(mpu6050_i2c_bus, &msgs, 1) == 1)
    {
        return RT_EOK;
    }
    else
    {
        return -RT_ERROR;
    }
}

#elif METHOD == 1

//дmpu6050�����Ĵ���
//reg:�Ĵ�����ַ
//data:����
//����ֵ: 0,���� / -1,�������
rt_err_t mpu6050_write_reg(rt_uint8_t reg, rt_uint8_t data)
{
    rt_uint8_t buf[2];

    buf[0] = reg;
    buf[1] = data;

    if (rt_i2c_master_send(mpu6050_i2c_bus, MPU6050_ADDR, 0, buf, 2) == 2)
    {
        return RT_EOK;
    }
    else
    {
        return -RT_ERROR;
    }
}

#endif

#if METHOD == 0

//��ȡ�Ĵ�������
//reg:Ҫ��ȡ�ļĴ�����ַ
//len:Ҫ��ȡ�������ֽ���
//buf:��ȡ�������ݴ洢��
//����ֵ: 0,���� / -1,�������
rt_err_t mpu6050_read_reg(rt_uint8_t reg, rt_uint8_t len, rt_uint8_t *buf)
{
    struct rt_i2c_msg msgs[2];

    msgs[0].addr  = MPU6050_ADDR;    /* �ӻ���ַ */
    msgs[0].flags = RT_I2C_WR;       /* д��־ */
    msgs[0].buf   = &reg;            /* �ӻ��Ĵ�����ַ */
    msgs[0].len   = 1;               /* ���������ֽ��� */

    msgs[1].addr  = MPU6050_ADDR;    /* �ӻ���ַ */
    msgs[1].flags = RT_I2C_RD;       /* ����־ */
    msgs[1].buf   = buf;             /* ��ȡ����ָ�� */
    msgs[1].len   = len;             /* ��ȡ�����ֽ��� */

    if (rt_i2c_transfer(mpu6050_i2c_bus, msgs, 2) == 2)
    {
        return RT_EOK;
    }
    else
    {
        return -RT_ERROR;
    }
}

#elif METHOD == 1

//��ȡ�Ĵ�������
//reg:Ҫ��ȡ�ļĴ�����ַ
//len:Ҫ��ȡ�������ֽ���
//buf:��ȡ�������ݴ洢��
//����ֵ: 0,���� / -1,�������
rt_err_t mpu6050_read_reg(rt_uint8_t reg, rt_uint8_t len, rt_uint8_t *buf)
{
    if (rt_i2c_master_send(mpu6050_i2c_bus, MPU6050_ADDR, 0, &reg, 1) == 1)
    {
        if (rt_i2c_master_recv(mpu6050_i2c_bus, MPU6050_ADDR, 0, buf, len) == len)
        {
            return RT_EOK;
        }
        else
        {
            return -RT_ERROR;
        }
    }
    else
    {
        return -RT_ERROR;
    }

}

#endif

//����MPU6050�����Ǵ����������̷�Χ
//fsr:0,��250dps;1,��500dps;2,��1000dps;3,��2000dps
//����ֵ: 0,���óɹ� / -1,����ʧ��
rt_err_t mpu6050_gyro_fsr_set(rt_uint8_t fsr)
{
    return mpu6050_write_reg(MPU_GYRO_CFG_REG, fsr << 3);//���������������̷�Χ
}

//����MPU6050���ٶȴ����������̷�Χ
//fsr:0,��2g;1,��4g;2,��8g;3,��16g
//����ֵ: 0,���óɹ� / -1,����ʧ��
rt_err_t mpu6050_accel_fsr_set(rt_uint8_t fsr)
{
    return mpu6050_write_reg(MPU_ACCEL_CFG_REG, fsr << 3);//���ü��ٶȴ����������̷�Χ
}

//����MPU6050�����ֵ�ͨ�˲���
//lpf:���ֵ�ͨ�˲�Ƶ��(Hz)
//����ֵ: 0,���óɹ� / -1,����ʧ��
rt_err_t mpu6050_lpf_set(rt_uint16_t lpf)
{
    rt_uint8_t data = 0;

    if (lpf >= 188)
    {
        data = 1;
    }
    else if (lpf >= 98)
    {
        data = 2;
    }
    else if (lpf >= 42)
    {
        data = 3;
    }
    else if (lpf >= 20)
    {
        data = 4;
    }
    else if (lpf >= 10)
    {
        data = 5;
    }
    else
    {
        data = 6;
    }
    return mpu6050_write_reg(MPU_CFG_REG, data);//�������ֵ�ͨ�˲���
}

//����MPU6050�Ĳ�����(�ٶ�Fs=1KHz)
//rate:4~1000(Hz)
//����ֵ: 0,���óɹ� / -1,����ʧ��
rt_err_t mpu6050_sample_rate_set(rt_uint16_t rate)
{
    rt_uint8_t data;

    if (rate > 1000)
    {
        rate = 1000;
    }
    if (rate < 4)
    {
        rate = 4;
    }
    data = 1000 / rate - 1;
    data = mpu6050_write_reg(MPU_SAMPLE_RATE_REG, data);    //���ò�����
    return mpu6050_lpf_set(rate / 2);  //�Զ�����LPFΪ�����ʵ�һ��
}

//�õ��¶�ֵ
//temperature:�¶�ֵ(������100��)
//����ֵ: 0,�ɹ� / -1,ʧ��
rt_err_t mpu6050_temperature_get(rt_int16_t *temperature)
{
    rt_uint8_t buf[2];
    rt_int16_t raw;
    float temp;
    rt_err_t ret;

    ret = mpu6050_read_reg(MPU_TEMP_OUTH_REG, 2, buf);
    if (ret == RT_EOK)
    {
        raw = ((rt_uint16_t)buf[0] << 8) | buf[1];
        temp = 36.53 + ((double)raw) / 340;
        *temperature = temp * 100;

        return RT_EOK;
    }
    else
    {
        return -RT_ERROR;
    }
}

//�õ�������ֵ(ԭʼֵ)
//gx,gy,gz:������x,y,z���ԭʼ����(������)
//����ֵ: 0,�ɹ� / -1,�������
rt_err_t mpu6050_gyroscope_get(rt_int16_t *gx, rt_int16_t *gy, rt_int16_t *gz)
{
    rt_uint8_t buf[6], ret;

    ret = mpu6050_read_reg(MPU_GYRO_XOUTH_REG, 6, buf);
    if (ret == 0)
    {
        *gx = ((rt_uint16_t)buf[0] << 8) | buf[1];
        *gy = ((rt_uint16_t)buf[2] << 8) | buf[3];
        *gz = ((rt_uint16_t)buf[4] << 8) | buf[5];

        return RT_EOK;
    }
    else
    {
        return -RT_ERROR;
    }
}

//�õ����ٶ�ֵ(ԭʼֵ)
//gx,gy,gz:������x,y,z���ԭʼ����(������)
//����ֵ: 0,�ɹ� / -1,�������
rt_err_t mpu6050_accelerometer_get(rt_int16_t *ax, rt_int16_t *ay, rt_int16_t *az)
{
    rt_uint8_t buf[6], ret;

    ret = mpu6050_read_reg(MPU_ACCEL_XOUTH_REG, 6, buf);
    if (ret == 0)
    {
        *ax = ((rt_uint16_t)buf[0] << 8) | buf[1];
        *ay = ((rt_uint16_t)buf[2] << 8) | buf[3];
        *az = ((rt_uint16_t)buf[4] << 8) | buf[5];

        return RT_EOK;
    }
    else
    {
        return -RT_ERROR;
    }
}
/* end of mpu6050 driver */

//MPU6050Ӳ����ʼ��
//����ֵ: 0,�ɹ� / -1,�������
int mpu6050_hw_init(void)
{
    rt_uint8_t res;

    mpu6050_i2c_bus = rt_i2c_bus_device_find(MPU6050_I2CBUS_NAME);  /*����I2C�豸*/

    if (mpu6050_i2c_bus == RT_NULL)
    {
        MPUDEBUG("can't find mpu6050 %s device\r\n", MPU6050_I2CBUS_NAME);
        return -RT_ERROR;
    }

    MPUDEBUG("mpu6050 set i2c bus to %s\r\n", MPU6050_I2CBUS_NAME);

    //��λMPU6050
    mpu6050_write_reg(MPU_PWR_MGMT1_REG, 0x80);
    //delay a while
    rt_thread_delay(rt_tick_from_millisecond(20));
    //����MPU6050
    mpu6050_write_reg(MPU_PWR_MGMT1_REG, 0x00);
    //delay a while
    rt_thread_delay(rt_tick_from_millisecond(20));
    //�����Ǵ�����,��2000dps
    mpu6050_gyro_fsr_set(3);
    //���ٶȴ�����,��2g
    mpu6050_accel_fsr_set(0);
    //�ر������ж�
    mpu6050_write_reg(MPU_INT_EN_REG, 0x00);
    //I2C��ģʽ�ر�
    mpu6050_write_reg(MPU_USER_CTRL_REG, 0x00);
    //�ر�FIFO
    mpu6050_write_reg(MPU_FIFO_EN_REG, 0x00);
    //INT���ŵ͵�ƽ��Ч
    mpu6050_write_reg(MPU_INTBP_CFG_REG, 0x80);

    //��WHO_AM_I�Ĵ���
    mpu6050_read_reg(MPU_DEVICE_ID_REG, 1, &res);
    if (res == MPU6050_ADDR)
    {
        MPUDEBUG("read mpu6050 id ok: 0x%02x\r\n", res);
        //����CLKSEL,PLL X��Ϊ�ο�
        mpu6050_write_reg(MPU_PWR_MGMT1_REG, 0x01);
        //���ٶ��������Ƕ�����
        mpu6050_write_reg(MPU_PWR_MGMT2_REG, 0x00);
        //���ò�����Ϊ125Hz
        mpu6050_sample_rate_set(125);
    }
    else
    {
        MPUDEBUG("read mpu6050 id error: 0x%02x\r\n", res);
        return -RT_ERROR;
    }

    return RT_EOK;
}
INIT_APP_EXPORT(mpu6050_hw_init);



