
#include "main.h"

#define WE1 GPIOA,GPIO_PIN_8		//	port A
#define WE2 GPIOA,GPIO_PIN_9
#define WE3 GPIOA,GPIO_PIN_10
#define WE4 GPIOA,GPIO_PIN_11

//++++++++++++++++++++++++++ output enable ++++++++++++++++++++++++++++++++++++++++++++++++++++
#define OE GPIOB,GPIO_PIN_15		//	port B

//++++++++++++++++++++++++++ chip select enable +++++++++++++++++++++++++++++++++++++++++++++++
#define CS1 GPIOB,GPIO_PIN_2		//	port B
#define CS2 GPIOB,GPIO_PIN_5
#define CS3 GPIOB,GPIO_PIN_6
#define CS4 GPIOB,GPIO_PIN_7

//++++++++++++++++++++++++++ address lines enable +++++++++++++++++++++++++++++++++++++++++++++

#define A0  GPIOC,GPIO_PIN_0	//	port C
#define A1  GPIOC,GPIO_PIN_1	
#define A2  GPIOC,GPIO_PIN_2	
#define A3  GPIOC,GPIO_PIN_3
#define A4  GPIOC,GPIO_PIN_4		
#define A5  GPIOC,GPIO_PIN_5	
#define A6  GPIOC,GPIO_PIN_6	
#define A7  GPIOC,GPIO_PIN_7
#define A8  GPIOC,GPIO_PIN_8		//	port C
#define A9  GPIOC,GPIO_PIN_9		
#define A10 GPIOC,GPIO_PIN_10	
#define A11 GPIOC,GPIO_PIN_11
#define A12 GPIOC,GPIO_PIN_12

#define A13 GPIOB,GPIO_PIN_8		//	port B
#define A14 GPIOB,GPIO_PIN_9
#define A15 GPIOB,GPIO_PIN_10
#define A16 GPIOB,GPIO_PIN_11
#define A17 GPIOB,GPIO_PIN_12
#define A18 GPIOB,GPIO_PIN_13

/*

#define A0  GPIOB,GPIO_PIN_11		//	port C
#define A1  GPIOC,GPIO_PIN_0	
#define A2  GPIOC,GPIO_PIN_1	
#define A3  GPIOC,GPIO_PIN_2
#define A4  GPIOC,GPIO_PIN_3		
#define A5  GPIOC,GPIO_PIN_4	
#define A6  GPIOC,GPIO_PIN_5	
#define A7  GPIOC,GPIO_PIN_6
#define A8  GPIOC,GPIO_PIN_9		//	port C
#define A9  GPIOC,GPIO_PIN_11		
#define A10 GPIOC,GPIO_PIN_10	
#define A11 GPIOB,GPIO_PIN_10	
#define A12 GPIOC,GPIO_PIN_7
#define A13 GPIOC,GPIO_PIN_8		//	port B
#define A14 GPIOB,GPIO_PIN_8
#define A15 GPIOC,GPIO_PIN_12
#define A16 GPIOB,GPIO_PIN_9
#define A17 GPIOB,GPIO_PIN_13
#define A18 GPIOB,GPIO_PIN_12
*/

#define LOW  GPIO_PIN_RESET
#define HIGH GPIO_PIN_SET		

UART_HandleTypeDef huart4;

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_UART4_Init(void);

static void config_Din(void);
static void config_Dout(void);
void goto_mem_addr(int addr);
void push_byte(char bank,char data);
char pop_byte(char bank);
void write_byte(int address,char bank,char data);
char read_byte(int address,char bank);//,short bytes
void proc_byte(int address,char bank,char data);
void chip_erase(char bank);


////////////////////////////////////////////
 void Serial_begin(uint32_t); //pass baug rate and start serial communiaction 
 void Serial_print_int (int); //pass integer value to print it on screen
 void Serial_print_char (char); //pass char value to print it on screen 
 void Serial_print_string (char[]); //pass string value to print it 
////////////////////////////////////////////////////
void Serial_print_char (char value)
 {
	 //UART1_SendData8(value);
	 HAL_UART_Transmit(&huart4,&value,1,10);
	 //while (UART1_GetFlagStatus(UART1_FLAG_TXE) == RESET); //wait for sending
 }
 
 
 void Serial_print_int (int number) //Funtion to print int value to serial monitor 
 {
	 char count = 0;
	 char digit[5] = "";
	 
	 while (number != 0) //split the int to char array 
	 {
		 digit[count] = number%10;
		 count++;
		 number = number/10;
	 }
	 
	 while (count !=0) //print char array in correct direction 
	 {
		 Serial_print_char(digit[count-1] + 0x30);	
		 count--;		 
	 }
 }
 

 void Serial_print_string (char string[])
 {

	 char i=0;

	 while (string[i] != 0x00)
	 {
		//UART1_SendData8(string[i]);
		HAL_UART_Transmit(&huart4,&string[i],1,10);					
		i++;
	}
 }



////////////////////////////////////////////////////
void goto_mem_addr(int addr){
	char buf=0;
		//if(addr <=524287){
															if(1<<0 & addr)buf=1;else buf=0;
		HAL_GPIO_WritePin(A0,buf);if(1<<1 & addr)buf=1;else buf=0;
		HAL_GPIO_WritePin(A1,buf);if(1<<2 & addr)buf=1;else buf=0;
		HAL_GPIO_WritePin(A2,buf);if(1<<3 & addr)buf=1;else buf=0;
		HAL_GPIO_WritePin(A3,buf);if(1<<4 & addr)buf=1;else buf=0;
		HAL_GPIO_WritePin(A4,buf);if(1<<5 & addr)buf=1;else buf=0;					  
		HAL_GPIO_WritePin(A5,buf);if(1<<6 & addr)buf=1;else buf=0;
		HAL_GPIO_WritePin(A6,buf);if(1<<7 & addr)buf=1;else buf=0;					  
		HAL_GPIO_WritePin(A7,buf);if(1<<8 & addr)buf=1;else buf=0;
		HAL_GPIO_WritePin(A8,buf);if(1<<9 & addr)buf=1;else buf=0;					  
		HAL_GPIO_WritePin(A9,buf);if(1<<10 & addr)buf=1;else buf=0;
		HAL_GPIO_WritePin(A10,buf);if(1<<11 & addr)buf=1;else buf=0;					  
		HAL_GPIO_WritePin(A11,buf);if(1<<12 & addr)buf=1;else buf=0;
		HAL_GPIO_WritePin(A12,buf);if(1<<13 & addr)buf=1;else buf=0;					  
		HAL_GPIO_WritePin(A13,buf);if(1<<14 & addr)buf=1;else buf=0;
		HAL_GPIO_WritePin(A14,buf);if(1<<15 & addr)buf=1;else buf=0;		
		HAL_GPIO_WritePin(A15,buf);if(1<<16 & addr)buf=1;else buf=0;		
		HAL_GPIO_WritePin(A16,buf);if(1<<17 & addr)buf=1;else buf=0;		
		HAL_GPIO_WritePin(A17,buf);if(1<<18 & addr)buf=1;else buf=0;		
		HAL_GPIO_WritePin(A18,buf);		
			
	//}

}
void push_byte(char bank,char data){
	char i,j,temp=0;

	for(i=0;i<8;i++){
		HAL_GPIO_WritePin(GPIOE,1<<i,1<<i &data);		
		temp|=1<<i &data;
	}			
	Serial_print_int(temp);Serial_print_string(" written\n");
}


char pop_byte(char bank){
	char i,data=0;	
	
	for(i=0;i<8;i++){
		if(HAL_GPIO_ReadPin(GPIOE,1<<i))data= data |(1<<i);				
	}			
	Serial_print_int(data);Serial_print_string(" from flash\n");		
	return data;
}


void write_byte(int address,char bank,char data){//,short bytes GPIO_PIN_RESET @arg GPIO_PIN_SET:
	//data bus config
	HAL_GPIO_WritePin(OE,HIGH);
	HAL_GPIO_WritePin(WE1,HIGH);
	HAL_GPIO_WritePin(WE2,HIGH);
	HAL_GPIO_WritePin(WE3,HIGH);
	HAL_GPIO_WritePin(WE4,HIGH);
	
	goto_mem_addr(address);//address setup	
	push_byte(bank,data);			//write_byte data on port
  
	if(bank==0)HAL_GPIO_WritePin(CS1,LOW); //enable pin chipselect
	else if(bank==1)HAL_GPIO_WritePin(CS2,LOW); //enable pin chipselect
	else if(bank==2)HAL_GPIO_WritePin(CS3,LOW); //enable pin chipselect
	else if(bank==3)HAL_GPIO_WritePin(CS4,LOW); //enable pin chipselect
		
	
	     if(bank==0)HAL_GPIO_WritePin(WE1,LOW);
	else if(bank==1)HAL_GPIO_WritePin(WE2,LOW);
	else if(bank==2)HAL_GPIO_WritePin(WE3,LOW);
	else if(bank==3)HAL_GPIO_WritePin(WE4,LOW);
	
	HAL_Delay(1);	
	HAL_GPIO_WritePin(WE1,HIGH);
	HAL_GPIO_WritePin(WE2,HIGH);
	HAL_GPIO_WritePin(WE3,HIGH);
	HAL_GPIO_WritePin(WE4,HIGH);
	//disarm chip
	HAL_GPIO_WritePin(CS1,HIGH);
	HAL_GPIO_WritePin(CS2,HIGH);
	HAL_GPIO_WritePin(CS3,HIGH);
	HAL_GPIO_WritePin(CS4,HIGH);
	
}
void proc_byte(int address,char bank,char data){
	config_Dout();//data bus config
	write_byte(0x5555,bank,0xA0);	
	write_byte(address,bank,data);
	
	HAL_Delay(1);
}

char read_byte(int address,char bank){//,short bytes
	char byte;
	config_Din();//data bus config
		
	HAL_GPIO_WritePin(WE1,HIGH);
	HAL_GPIO_WritePin(WE2,HIGH);
	HAL_GPIO_WritePin(WE3,HIGH);
	HAL_GPIO_WritePin(WE4,HIGH);
	HAL_GPIO_WritePin(OE,HIGH);
	
	goto_mem_addr(address);//address setup
	
	if(bank==0)HAL_GPIO_WritePin(CS1,LOW); //enable pin chipselect
	else if(bank==1)HAL_GPIO_WritePin(CS2,LOW); //enable pin chipselect
	else if(bank==2)HAL_GPIO_WritePin(CS3,LOW); //enable pin chipselect
	else if(bank==3)HAL_GPIO_WritePin(CS4,LOW); //enable pin chipselect
	
	HAL_GPIO_WritePin(OE,LOW);   //out disable  set- high	
	HAL_Delay(1);
	
	byte=pop_byte(bank);
	
	//disarm chip
	HAL_GPIO_WritePin(OE,HIGH);
	HAL_GPIO_WritePin(CS1,HIGH);
	HAL_GPIO_WritePin(CS2,HIGH);
	HAL_GPIO_WritePin(CS3,HIGH);
	HAL_GPIO_WritePin(CS4,HIGH);
	return byte;
}

void chip_erase(char bank){
	config_Dout();//data bus config
	write_byte(0x5555,bank,0xAA);
	write_byte(0x2AAA,bank,0x55);
	write_byte(0x5555,bank,0x80);
	write_byte(0x5555,bank,0xAA);
	write_byte(0x2AAA,bank,0x55);
	write_byte(0x5555,bank,0x10);
	HAL_Delay(1);
	config_Din();//data bus config
}


int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_UART4_Init();
	//chip_erase(0);
	
	proc_byte(60,0,40);
	
	
	
	read_byte(60,0);
	
	
  while (1)
  {
		//Serial_print_string("bumchick 123\n");		Serial_print_int(512);
		//Serial_print_string("read val \n");
			
		//Serial_print_int(temp);
	
		HAL_Delay(2000);			
  }
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

static void MX_UART4_Init(void)
{

  /* USER CODE BEGIN UART4_Init 0 */

  /* USER CODE END UART4_Init 0 */

  /* USER CODE BEGIN UART4_Init 1 */

  /* USER CODE END UART4_Init 1 */
  huart4.Instance = UART4;
  huart4.Init.BaudRate = 115200;
  huart4.Init.WordLength = UART_WORDLENGTH_8B;
  huart4.Init.StopBits = UART_STOPBITS_1;
  huart4.Init.Parity = UART_PARITY_NONE;
  huart4.Init.Mode = UART_MODE_TX_RX;
  huart4.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart4.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart4) != HAL_OK)
  {
    Error_Handler();
  }

}

static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7
                          |GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11
                          |GPIO_PIN_12, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12
                          |GPIO_PIN_13|GPIO_PIN_15|GPIO_PIN_5|GPIO_PIN_6
                          |GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11, GPIO_PIN_RESET);	

//address lines
  /*Configure GPIO pins : PC0 PC1 PC2 PC3
                           PC4 PC5 PC6 PC7
                           PC8 PC9 PC10 PC11
                           PC12 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7
                          |GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11
                          |GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PB2 PB10 PB11 PB12
                           PB13 PB15 PB5 PB6
                           PB7 PB8 PB9 */
  GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12
                          |GPIO_PIN_13|GPIO_PIN_15|GPIO_PIN_5|GPIO_PIN_6
                          |GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);


  /*Configure GPIO pins : PA8 PA9 PA10 PA11 */
  GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

static void config_Din(){
	__HAL_RCC_GPIOE_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	GPIO_InitTypeDef GPIO_InitStruct = {0};
HAL_GPIO_WritePin(GPIOE,GPIO_PIN_All, GPIO_PIN_RESET);	
HAL_GPIO_WritePin(GPIOD,GPIO_PIN_All, GPIO_PIN_RESET);	
//datalines start
  /*Configure GPIO pins : PE2 PE3 PE4 PE5
                           PE6 PE7 PE8 PE9
                           PE10 PE11 PE12 PE13
                           PE14 PE15 PE0 PE1 */
  GPIO_InitStruct.Pin =   GPIO_PIN_All;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed= GPIO_SPEED_HIGH;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
	
	  /*Configure GPIO pins : PD8 PD9 PD10 PD11
                           PD12 PD13 PD14 PD15
                           PD0 PD1 PD2 PD3
                           PD4 PD5 PD6 PD7 */
  GPIO_InitStruct.Pin = GPIO_PIN_All;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed= GPIO_SPEED_HIGH;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
	//data lines end

}

static void config_Dout(){
	__HAL_RCC_GPIOE_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_All, GPIO_PIN_RESET);
  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD,GPIO_PIN_All, GPIO_PIN_RESET);	
	
	GPIO_InitStruct.Pin = GPIO_PIN_All;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = GPIO_PIN_All;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
}

void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT sk0512 *****END OF FILE****/
