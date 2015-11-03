extern "C"{
#include <delay.h>
#include <FillPat.h>
#include <I2CEEPROM.h>
#include <LaunchPad.h>
#include <OrbitBoosterPackDefs.h>
#include <OrbitOled.h>
#include <OrbitOledChar.h>
#include <OrbitOledGrph.h>
}


/* ------------------------------------------------------------ */
/*				Booster Pack Variables          */
//* ------------------------------------------------------------ */

// Global variables 
extern int xchOledMax; // defined in OrbitOled.c
extern int ychOledMax; // defined in OrbitOled.c

// Local variables
char    key;
char	chSwtCur;
char	chSwtPrev;
bool	fClearOled;


/* ------------------------------------------------------------ */
/*				Forward Declarations							*/
/* ------------------------------------------------------------ */
void DeviceInit();
void DisplayKeys();



/* ------------------------------------------------------------ */
/*				Setup            		*/
//* ------------------------------------------------------------ */

void setup(){
  Serial.begin(9600);
  // Configure all components of Orbit Booster Pack
  
  // For OLED display, the only method req'd is OrbitOledInit()
  DeviceInit();
}

void DeviceInit()
{
  /*
   * First, Set Up the Clock.
   * Main OSC		  -> SYSCTL_OSC_MAIN
   * Runs off 16MHz clock -> SYSCTL_XTAL_16MHZ
   * Use PLL		  -> SYSCTL_USE_PLL
   * Divide by 4	  -> SYSCTL_SYSDIV_4
   */
//  SysCtlClockSet(SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ | SYSCTL_USE_PLL | SYSCTL_SYSDIV_4);
//
//  /*
//   * Enable and Power On All GPIO Ports
//   */
//  //SysCtlPeripheralEnable(	SYSCTL_PERIPH_GPIOA | SYSCTL_PERIPH_GPIOB | SYSCTL_PERIPH_GPIOC |
//  //						SYSCTL_PERIPH_GPIOD | SYSCTL_PERIPH_GPIOE | SYSCTL_PERIPH_GPIOF);
//
//  SysCtlPeripheralEnable(	SYSCTL_PERIPH_GPIOA );
//  SysCtlPeripheralEnable(	SYSCTL_PERIPH_GPIOB );
//  SysCtlPeripheralEnable(	SYSCTL_PERIPH_GPIOC );
//  SysCtlPeripheralEnable(	SYSCTL_PERIPH_GPIOD );
//  SysCtlPeripheralEnable(	SYSCTL_PERIPH_GPIOE );
//  SysCtlPeripheralEnable(	SYSCTL_PERIPH_GPIOF );
//  /*
//   * Pad Configure.. Setting as per the Button Pullups on
//   * the Launch pad (active low).. changing to pulldowns for Orbit
//   */
//  GPIOPadConfigSet(SWTPort, SWT1 | SWT2, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPD);
//
//  GPIOPadConfigSet(BTN1Port, BTN1, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPD);
//  GPIOPadConfigSet(BTN2Port, BTN2, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPD);
//
//  GPIOPadConfigSet(LED1Port, LED1, GPIO_STRENGTH_8MA_SC, GPIO_PIN_TYPE_STD);
//  GPIOPadConfigSet(LED2Port, LED2, GPIO_STRENGTH_8MA_SC, GPIO_PIN_TYPE_STD);
//  GPIOPadConfigSet(LED3Port, LED3, GPIO_STRENGTH_8MA_SC, GPIO_PIN_TYPE_STD);
//  GPIOPadConfigSet(LED4Port, LED4, GPIO_STRENGTH_8MA_SC, GPIO_PIN_TYPE_STD);
//
//  /*
//   * Initialize Switches as Input
//   */
//  GPIOPinTypeGPIOInput(SWTPort, SWT1 | SWT2);
//
//  /*
//   * Initialize Buttons as Input
//   */
//  GPIOPinTypeGPIOInput(BTN1Port, BTN1);
//  GPIOPinTypeGPIOInput(BTN2Port, BTN2);
//
//  /*
//   * Initialize LEDs as Output
//   */
//  GPIOPinTypeGPIOOutput(LED1Port, LED1);
//  GPIOPinTypeGPIOOutput(LED2Port, LED2);
//  GPIOPinTypeGPIOOutput(LED3Port, LED3);
//  GPIOPinTypeGPIOOutput(LED4Port, LED4);
//
//  /*
//   * Enable ADC Periph
//   */
//  SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
//
//  GPIOPinTypeADC(AINPort, AIN);
//
//  /*
//   * Enable ADC with this Sequence
//   * 1. ADCSequenceConfigure()
//   * 2. ADCSequenceStepConfigure()
//   * 3. ADCSequenceEnable()
//   * 4. ADCProcessorTrigger();
//   * 5. Wait for sample sequence ADCIntStatus();
//   * 6. Read From ADC
//   */
//  ADCSequenceConfigure(ADC0_BASE, 0, ADC_TRIGGER_PROCESSOR, 0);
//  ADCSequenceStepConfigure(ADC0_BASE, 0, 0, ADC_CTL_IE | ADC_CTL_END | ADC_CTL_CH0);
//  ADCSequenceEnable(ADC0_BASE, 0);

  /*
   * Initialize the OLED
   */
  OrbitOledInit();

  /*
   * Reset flags
   */
  chSwtCur = 0;
  chSwtPrev = 0;
  fClearOled = true;

}


/* ------------------------------------------------------------ */
/*				Loop              		*/
//* ------------------------------------------------------------ */
void loop(){
  Wire.requestFrom(2, 6);
    while(Wire.available())    // slave may send less than requested
  { 
    char c = Wire.read(); // receive a byte as character
    Serial.print(c);         // print the character
  }

  delay(500);
//  DisplayKeys();
}

void DisplayKeys(){ 
  
  char hello[] = { 
    'H', 'e', 'l', 'l', 'o', '\0'   };

  if(fClearOled == true) {
    OrbitOledClear();
    OrbitOledMoveTo(0,0);
    OrbitOledSetCursor(0,0);
    fClearOled = false;
  }
 
  
  // OrbitOledPutChar() to display chars
  
  // OrbitOledPutString() to display char arrays
  
  // OrbitOledSetCursor(col,row) used for chars/strings
  // There are 16 cols and 4 rows
  // Therefore if you want a string on the second row
  // Do OrbitOledSetCursor(0,1) before OrbitOledPutString()
  
  // If you do successive OrbitOledPutString(), the words will be appended
  // after each other (no spaces)
  
  OrbitOledSetCursor(0, 0);

  OrbitOledPutString(hello);
  OrbitOledPutString(hello);
  OrbitOledPutString(hello);

}


