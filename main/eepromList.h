void eepromDataSave()
{
	for(int savenum=0;savenum<20;savenum++){
		if(savenum==7)
			EEPROM.write(1+savenum,-setValue[savenum]);
		else
			EEPROM.write(1+savenum,setValue[savenum]);
	}
	EEPROM.commit();
}
void eepromDataLoad()
{
	for(int savenum=0;savenum<20;savenum++){
		if(savenum==7)
			setValue[savenum] = -EEPROM.read(1+savenum);
		else
			setValue[savenum] = EEPROM.read(1+savenum);
	}
}