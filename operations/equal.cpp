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

#include "equal.h"

/*** Constructor ***/
Equal::Equal()
{
	setArgument1(0);
	setArgument2(0);
}

/*** Constructor ***/
Equal::Equal(Object* pArg1, Object* pArg2)
{
	setArgument1(pArg1);
	setArgument2(pArg2);
}

/*** Get this operation's type ***/
unsigned char Equal::getType()
{
	return OP_EQUAL;
}

/*** Set the first argument ***/
void Equal::setArgument1(Object* pArg)
{
	arg1 = pArg;
}

/*** Set the second argument ***/
void Equal::setArgument2(Object* pArg)
{
	arg2 = pArg;
}

/*** Evaluate this object ***/
Object* Equal::evaluate()
{
	if(arg1 == 0)
		throw MissingArgumentException(getLineNumber(), getColumnNumber(), OBJ_INTEGER | OBJ_REAL | OBJ_SEQUENCE | OBJ_TEXT | OBJ_LOGICAL, 1);
	if(arg2 == 0)
		throw MissingArgumentException(getLineNumber(), getColumnNumber(), OBJ_INTEGER | OBJ_REAL | OBJ_SEQUENCE | OBJ_TEXT | OBJ_LOGICAL, 2);

	std::unique_ptr<Object> obj1(arg1->evaluate());
	std::unique_ptr<Object> obj2(arg2->evaluate());

	if(obj1->getType() == OBJ_EMPTY && obj2->getType() == OBJ_EMPTY)
		return new Logical(true);

	if(obj1->getType() == OBJ_EMPTY || obj2->getType() == OBJ_EMPTY)
		return new Logical(false);

	if(obj1->getType() == OBJ_LOGICAL)
	{
		if(obj2->getType() != OBJ_LOGICAL)
			throw InvalidTypeException(getLineNumber(), getColumnNumber(), OBJ_LOGICAL, obj2->getType(), 2);

		Logical* cast1 = static_cast<Logical*>(obj1.get());
		Logical* cast2 = static_cast<Logical*>(obj2.get());
		return new Logical(cast1->getValue() == cast2->getValue());
	}

	if(obj1->getType() == OBJ_INTEGER)
	{
		Integer* cast1 = static_cast<Integer*>(obj1.get());

		if(obj2->getType() == OBJ_INTEGER)
		{
			Integer* cast2 = static_cast<Integer*>(obj2.get());
			return new Logical(cast1->getValue() == cast2->getValue());
		}

		if(obj2->getType() == OBJ_REAL)
		{
			Real* cast2 = static_cast<Real*>(obj2.get());
			return new Logical(cast1->getValue() == cast2->getValue());
		}
		
		throw InvalidTypeException(getLineNumber(), getColumnNumber(), OBJ_INTEGER | OBJ_REAL, obj2->getType(), 2);
	}

	if(obj1->getType() == OBJ_REAL)
	{
		Real* cast1 = static_cast<Real*>(obj1.get());

		if(obj2->getType() == OBJ_INTEGER)
		{
			Integer* cast2 = static_cast<Integer*>(obj2.get());
			return new Logical(cast1->getValue() == cast2->getValue());
		}

		if(obj2->getType() == OBJ_REAL)
		{
			Real* cast2 = static_cast<Real*>(obj2.get());
			return new Logical(cast1->getValue() == cast2->getValue());
		}
		
		throw InvalidTypeException(getLineNumber(), getColumnNumber(), OBJ_INTEGER | OBJ_REAL, obj2->getType(), 2);
	}

	if(obj1->getType() == OBJ_TEXT)
	{
		if(obj2->getType() != OBJ_TEXT)
			throw InvalidTypeException(getLineNumber(), getColumnNumber(), OBJ_TEXT, obj2->getType(), 2);

		Text* cast1 = static_cast<Text*>(obj1.get());
		Text* cast2 = static_cast<Text*>(obj2.get());
		return new Logical(cast1->getValue().compare(cast2->getValue()) == 0);
	}

	if(obj1->getType() == OBJ_SEQUENCE)
	{
		if(obj2->getType() != OBJ_SEQUENCE)
			throw InvalidTypeException(getLineNumber(), getColumnNumber(), OBJ_SEQUENCE, obj2->getType(), 2);

		Sequence* cast1 = static_cast<Sequence*>(obj1.get());
		Sequence* cast2 = static_cast<Sequence*>(obj2.get());

		if(cast1->getLength() != cast2->getLength())
			return new Logical(false);

		for(unsigned int i = 0; i < cast1->getLength(); i++)
		{
			std::unique_ptr<Object> eqResult(Equal(cast1->getObject(i)->clone(), cast2->getObject(i)->clone()).evaluate());
			Logical* logResult = static_cast<Logical*>(eqResult.get());
			if(logResult->getValue() == false)
				return new Logical(false);
		}

		return new Logical(true);
	}

	throw InvalidTypeException(getLineNumber(), getColumnNumber(), OBJ_LOGICAL | OBJ_REAL | OBJ_TEXT | OBJ_SEQUENCE | OBJ_INTEGER, obj1->getType(), 1);
	return 0;
}

/*** Destructor ***/
Equal::~Equal()
{
	delete arg1;
	delete arg2;
}
