/**
 * @file /sleo_screen_driver/src/sleo_screen_driver.cpp
 *
 * @brief Implementation for dirver with read data from Sleo screen nodelet
 *
 * @author Carl
 *
 **/

/*****************************************************************************
 ** Includes
 *****************************************************************************/
#include "sleo_screen_driver/sleo_screen_driver.hpp"

namespace sleo_screen_driver{

  SleoScreenDriver::SleoScreenDriver(std::string serialNumber, int baudRate) : serialNumber_(serialNumber),baudRate_(baudRate),system_voltage_(36.0),system_ampere_(15.0),system_temperature_(25.0),system_fault_(0),system_status_(0),left_temperature_(25.0),left_velocity_(2500.0),left_voltage_(36.0),left_ampere_(10.0),right_temperature_(25.0),right_velocity_(2500.0),right_voltage_(36.0),right_ampere_(10.0),system_fault("����"),system_status("����"){}

  SleoScreenDriver::~SleoScreenDriver(){
  }

  void SleoScreenDriver::Init(){

    fd = open(serialNumber_.c_str(), O_RDWR | O_NOCTTY | O_NDELAY );
    if(fd < 0){
      fprintf(stderr,"Open Serial: %s Error!",serialNumber_.c_str());
      exit(0);
    }

    memset(result_buf, 0, sizeof(result_buf));
    sleodriver_.UART0_Init(fd,baudRate_,0,8,1,'N');

//    printf("Open serial: [ %s ] successful, with idex: %d.\n", serialNumber_.c_str(), fd);
  }

  void SleoScreenDriver::update(){
    memset(result_buf, 0, sizeof(result_buf));
    fd = open(serialNumber_.c_str(), O_RDWR | O_NOCTTY | O_NDELAY );
    sleodriver_.UART0_Init(fd,baudRate_,0,8,1,'N');
    char endbuf[] = ("\n");
    char screen_code[] = "CLS(23);SBC(23);SXY(10,10);BOX(0,0,420,240,15);BOX(2,2,418,239,0);BOXF(2,2,418,30,3);PL(2,30,418,30,0);SBC(3);DS24(170,4,'ƽ̨״̬',15);SBC(23);DS24(30,60,'36V',1);DS24(25,95,'��ѹ',3);DS24(195,60,'10A',1);DS24(190,95,'����',3);DS24(355,60,'25��',1);DS24(350,95,'�¶�',3);DS24(25,155,'����',1);DS24(25,195,'����',3);DS24(190,155,'����',1);DS24(190,195,'״̬',3);SXY(0,0);;SXY(440,10);BOX(0,0,400,440,15);BOX(2,2,398,439,0);BOXF(2,2,398,30,3);PL(2,30,398,30,0);SBC(3);DS24(170,6,'���״̬',15,0);SBC(23);DS24(168,48,'��',3,0);DS24(316,48,'��',3,0);PL(2,88,398,88,0);DS24(34,120,'�¶�',3,0);DS24(168,120,'25��',1,0);DS24(316,120,'25��',1,0);PL(2,176,398,176,0);DS24(34,208,'�ٶ�',3,0);DS24(140,208,'2500RPM',1,0);DS24(296,208,'2500RPM',1,0);PL(2,264,398,264,0);DS24(34,298,'��ѹ',3,0);DS24(168,298,'36V',1,0);DS24(316,298,'36V',1,0);PL(2,352,398,352,0);DS24(34,388,'����',3,0);DS24(168,388,'15A',1,0);DS24(316,388,'15A',1,0);PL(113,30,113,440,0);PL(246,30,246,440,0);SXY(0,0);;PIC(10,360,1);PIC(10,255,2);PIC(130,260,3);TPN(2);";
    ////Because of the buffer of screen is 1kb, so just do under lines and do not change. 
   sprintf(screen_code,"CLS(23);SBC(23);SXY(10,10);BOX(0,0,420,240,15);BOX(2,2,418,239,0);BOXF(2,2,418,30,3);PL(2,30,418,30,0);SBC(3);DS24(170,4,'ƽ̨״̬',15);SBC(23);DS24(30,60,'%.0fV',1);DS24(25,95,'��ѹ',3);DS24(195,60,'%.0fA',1);DS24(190,95,'����',3);DS24(355,60,'%.0f��',1);DS24(350,95,'�¶�',3);DS24(25,155,'%s',1);DS24(25,195,'����',3);DS24(190,155,'%s',1);DS24(190,195,'״̬',3);SXY(0,0);;SXY(440,10);BOX(0,0,400,440,15);BOX(2,2,398,439,0);BOXF(2,2,398,30,3);PL(2,30,398,30,0);SBC(3);DS24(170,6,'���״̬',15,0);SBC(23);DS24(168,48,'��',3,0);DS24(316,48,'��',3,0);PL(2,88,398,88,0);DS24(34,120,'�¶�',3,0);DS24(168,120,'%.0f��',1,0);DS24(316,120,'%.0f��',1,0);PL(2,176,398,176,0);DS24(34,208,'�ٶ�',3,0);DS24(140,208,'%.0fRPM',1,0);DS24(296,208,'%.0fRPM',1,0);PL(2,264,398,264,0);DS24(34,298,'��ѹ',3,0);DS24(168,298,'%.0fV',1,0);DS24(316,298,'%.0fV',1,0);PL(2,352,398,352,0);DS24(34,388,'����',3,0);DS24(168,388,'%.0fA',1,0);DS24(316,388,'%.0fA',1,0);PL(113,30,113,440,0);PL(246,30,246,440,0);SXY(0,0);;PIC(10,360,1);PIC(10,255,2);PIC(130,260,3);TPN(2);",system_voltage_, system_ampere_, system_temperature_, system_fault.c_str(),system_status.c_str(),left_temperature_,right_temperature_,left_velocity_,right_velocity_,left_voltage_,right_voltage_,left_ampere_,right_ampere_);

    strcat(screen_code ,endbuf);			//��ӻ��з�
    screen_len = sizeof(screen_code);
    len= sleodriver_.UART0_Send(fd,screen_code, screen_len);
    usleep(1000000);
    if(!len || (len != screen_len)){
        fprintf(stderr, "Send data error !! ");
        return ;    
    }
    len = sleodriver_.UART0_Recv(fd, result_buf,sizeof("OK"));  
    if(len != -1){  
        char laser_data[3];
        sscanf(result_buf, "%s", &laser_data);
//        fprintf("receive data: "<<laser_data<<"\n");
    }else{  
        fprintf(stdout, "cannot receive data\n");  
    }  

    sleodriver_.UART0_Close(fd);
  }

  void SleoScreenDriver::switchStatus(int systemFault, int systemStatus){
    ////@TODO ����,���������ֽ��޷�ˢ�£� MOSFET
    switch(systemFault){
      case FAULT_OVERHEAT:                   system_fault = "����";break;
      case FAULT_OVERVOLTAGE:                system_fault = "��ѹ";break;
      case FAULT_UNDERVOLTAGE:               system_fault = "Ƿѹ";break;
      case FAULT_SHORT_CIRCUIT:              system_fault = "��·";break;
      case FAULT_EMERGENCY_STOP:             system_fault = "��ͣ";break;
      case FAULT_SEPEX_EXCITATION_FAULT:     system_fault = "����";break;
      case FAULT_MOSFET_FAILURE:             system_fault = "MOSFET";break;
      case FAULT_STARTUP_CONFIG_FAULT:       system_fault = "����";break;
      default:                               system_fault = "����";break;
    }

    switch(systemStatus){
      case STATUS_SERIAL_MODE:                system_status = "����";break;
      case STATUS_PULSE_MODE:                 system_status = "����";break;
      case STATUS_ANALOG_MODE:                system_status = "ģ��";break;
      case STATUS_POWER_STAGE_OFF:            system_status = "�ϵ�";break;
      case STATUS_STALL_DETECTED:             system_status = "ʧ��";break;
      case STATUS_AT_LIMIT:                   system_status = "����";break;
      case STATUS_MICROBASIC_SCRIPT_RUNNING:  system_status = "�ű�";break;
      default:                                system_status = "����";break;
    }

  }

} //namespace dumpbot_serial_func
