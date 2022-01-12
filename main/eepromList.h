void eepromDataSave()
{
	for(int savenum=0;savenum<20;savenum++){
		EEPROM.write(1+savenum,setValue[savenum]);
	}
	EEPROM.commit();
}
void eepromDataLoad()
{
	for(int savenum=0;savenum<20;savenum++){
		setValue[savenum] = EEPROM.read(1+savenum);
	}
}