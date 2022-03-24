void eepromDataSave()
{
	for(int savenum=0;savenum<20;savenum++){
		if((savenum>=4 && savenum<=6 )|| savenum == 8){//temp
			EEPROM.write(1+savenum,setValue[savenum]-discount);
		}
		else{
			EEPROM.write(1+savenum,setValue[savenum]);
		}
	}
	EEPROM.commit();
}
void eepromDataLoad()
{
	for(int savenum=0;savenum<20;savenum++){
		if((savenum>=4 && savenum<=6 )|| savenum == 8){//temp
			setValue[savenum] = EEPROM.read(1+savenum)+discount;
		}
		else{
			setValue[savenum] = EEPROM.read(1+savenum);
		}
	}
}