 EXPORT SystemReset
 AREA delay50ns,CODE,READONLY
	MOV R0, #1              
	MSR FAULTMASK, R0    
	LDR R0, =0xE000ED0C  
	LDR R1, =0x05FA0004     
	STR R1, [R0]            
	deadloop:      
		B deadloop        
	END