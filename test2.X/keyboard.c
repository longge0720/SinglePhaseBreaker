#include "keyboard.h"
#include "config.h"
unsigned char KeySta = 1;//����״̬��Ĭ�ϰ����ɿ�
unsigned char AutoOrManualMode = AutoMode;//0:�ֶ�ģʽ 1���Զ�ģʽ
//������ʼ����������ʼ������Ϊ����
//Ӳ�����
//RA5
void KeyInit(void)
{
    TRISA   |= 0x01 << 5;
    PORTA   |= 0x01 << 5;//����1
    ANSELA  &= ~(0x01 << 5);//����IO
    WPUA    |= 0x01 << 5;//ʹ��������
}

void KeyDriver(void)
{
 	static unsigned char backup = 1;//����
    
    if(backup != KeySta)//״̬�ı���
    {
        if(KeySta == 0x00)//��ⰴ������
        {
            KeyAction(0x00);//�������¶�������
        }
        else
        {
            KeyAction(0x01);//��������������
        }
        backup = KeySta;
    }
}

//��ʱ���ж��н���ɨ��
void KeyScan(void)
{
 	static unsigned char KeyBuf = 0xff;

    KeyBuf = (KeyBuf << 1) | KEY;

	if(KeyBuf == 0xff)
    {
        KeySta = 0x01;
    }
    else if(KeyBuf == 0x00)
    {
        KeySta = 0x00;
    }
	
}

void KeyAction(unsigned char KeyCode)
{
    if(KeyCode == 0x00)//�������£��˹�ģʽ
    {
        AutoOrManualMode = ManualMode;
    }
    else if(KeyCode == 0x01)//���������Զ�ģʽ
    {
        AutoOrManualMode = AutoMode;
    }
#if 0
       if(MOTOR_INA  == 1)
        {
            MOTOR_INA = 0;
        }
        else
        {
            MOTOR_INA = 1;
        }
#endif
}
