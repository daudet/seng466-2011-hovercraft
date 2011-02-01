/*
 * Mirf2.cpp
 *
 *  Created on: Jan 31, 2011
 *      Author: cei
 */

#include "Mirf2.h"

void MIRFinit(byte select)
{
	Mirf.csnPin = 7;
	Mirf.cePin = 8;
	Mirf.init();
	if (select==1)
	{
		Mirf.setRADDR((byte *)"clie1");
		Mirf.setTADDR((byte *)"serv1");
	}
	else if (select==2)
	{
		Mirf.setTADDR((byte *)"clie1");
		Mirf.setRADDR((byte *)"serv1");
	}
	Mirf.payload = 4;
	Mirf.config();
}

void MIRFsend(byte* buffer)
{
	while(!Mirf.dataReady());
	Mirf.send(buffer);
}

void MIRFreceive(byte*)
{
	while(!Mirf.dataReady());
	Mirf.getData(WIFIdata);
}
