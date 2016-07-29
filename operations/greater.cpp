// ReRap Version 0.9
// Copyright 2011 Matthew Mikolay.
//
// This file is part of ReRap.
//
// ReRap is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// ReRap is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with ReRap.  If not, see <http://www.gnu.org/licenses/>.

#include "greater.h"

/*** Constructor ***/
Greater::Greater()
{
	setArgument1(0);
	setArgument2(0);
}

/*** Constructor ***/
Greater::Greater(Object* pArg1, Object* pArg2)
{
	setArgument1(pArg1);
	setArgument2(pArg2);
}

/*** Get this operation's type ***/
unsigned char Greater::getType()
{
	return OP_GREATER;
}

/*** Set the first argument ***/
void Greater::setArgument1(Object* pArg)
{
	arg1 = pArg;
}

/*** Set the second argument ***/
void Greater::setArgument2(Object* pArg)
{
	arg2 = pArg;
}

/*** Evaluate this object ***/
Object* Greater::evaluate()
{
	if(arg1 == 0)
		throw MissingArgumentException(getLineNumber(), getColumnNumber(), OBJ_INTEGER | OBJ_REAL, 1);
	if(arg2 == 0)
		throw MissingArgumentException(getLineNumber(), getColumnNumber(), OBJ_INTEGER | OBJ_REAL, 2);

	std::unique_ptr<Object> obj1(arg1->evaluate());
	std::unique_ptr<Object> obj2(arg2->evaluate());

	if(obj1->getType() == OBJ_INTEGER)
	{
		Integer* cast1 = static_cast<Integer*>(obj1.get());

		if(obj2->getType() == OBJ_INTEGER)
		{
			Integer* cast2 = static_cast<Integer*>(obj2.get());
			return new Logical(cast1->getValue() > cast2->getValue());
		}

		if(obj2->getType() == OBJ_REAL)
		{
			Real* cast2 = static_cast<Real*>(obj2.get());
			return new Logical(cast1->getValue() > cast2->getValue());
		}

		throw InvalidTypeException(getLineNumber(), getColumnNumber(), OBJ_REAL | OBJ_INTEGER, obj2->getType(), 2);
	}

	if(obj1->getType() == OBJ_REAL)
	{
		Real* cast1 = static_cast<Real*>(obj1.get());

		if(obj2->getType() == OBJ_INTEGER)
		{
			Integer* cast2 = static_cast<Integer*>(obj2.get());
			return new Logical(cast1->getValue() > cast2->getValue());
		}

		if(obj2->getType() == OBJ_REAL)
		{
			Real* cast2 = static_cast<Real*>(obj2.get());
			return new Logical(cast1->getValue() > cast2->getValue());
		}

		throw InvalidTypeException(getLineNumber(), getColumnNumber(), OBJ_REAL | OBJ_INTEGER, obj2->getType(), 2);
	}

	throw InvalidTypeException(getLineNumber(), getColumnNumber(), OBJ_REAL | OBJ_INTEGER, obj2->getType(), 1);
	return 0;
}

/*** Destructor ***/
Greater::~Greater()
{
	delete arg1;
	delete arg2;
}
