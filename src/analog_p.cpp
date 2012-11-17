/**************************************************************************
 *  Copyright 2012 KISS Institute for Practical Robotics                  *
 *                                                                        *
 *  This file is part of libkovan.                                        *
 *                                                                        *
 *  libkovan is free software: you can redistribute it and/or modify      *
 *  it under the terms of the GNU General Public License as published by  *
 *  the Free Software Foundation, either version 2 of the License, or     *
 *  (at your option) any later version.                                   *
 *                                                                        *
 *  libkovan is distributed in the hope that it will be useful,           *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *  GNU General Public License for more details.                          *
 *                                                                        *
 *  You should have received a copy of the GNU General Public License     *
 *  along with libkovan. Check the LICENSE file in the project root.      *
 *  If not, see <http://www.gnu.org/licenses/>.                           *
 **************************************************************************/

#include "analog_p.hpp"
#include "kovan_p.hpp"
#include "kovan_regs_p.hpp"
#include "nyi.h"

using namespace Private;

Analog::~Analog()
{
}

void Analog::setPullup(const unsigned char& port, const bool& pullup)
{
	nyi("Private::Analog::setPullup");
}

bool Analog::pullup(const unsigned char& port) const
{
	nyi("Private::Analog::pullup");
	return false;
}

unsigned short Analog::value(const unsigned char& port) const
{
	// FIXME: This is an abomination. We should never have to to flush internally.
	// Will be corrected later.
	
	if(port < 8 || port > 15) return 0xFFFF;
	
	unsigned short adc_val = 0xFFFF;

	Private::Kovan *kovan = Private::Kovan::instance();
	
	kovan->enqueueCommand(createWriteCommand(ADC_GO_T, 0));
	kovan->enqueueCommand(createWriteCommand(ADC_CHAN_T, port));
	kovan->enqueueCommand(createWriteCommand(ADC_GO_T, 1));
	kovan->enqueueCommand(createWriteCommand(ADC_GO_T, 0));
	kovan->flush();
	
	for(unsigned char i = 0; i < 2; ++i) {
		// Wait for ready
		do{
			kovan->flush();
		} while(!kovan->currentState().t[ADC_VALID_T]);

		// Read raw voltage
		adc_val = kovan->currentState().t[ADC_IN_T];
	}

	return adc_val;
}

Analog *Analog::instance()
{
	static Analog s_analog;
	return &s_analog;
}

Analog::Analog()
{
}

Analog::Analog(const Analog& ) {}
Analog& Analog::operator=(const Analog&) { return *this; }