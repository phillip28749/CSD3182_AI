/*!*****************************************************************************
\file functions.cpp
\author Chen Yen Hsun
\par DP email: c.yenhsun\@digipen.edu
\par Course: CS380
\par Section: A
\par Programming Assignment 11
\date 07-12-2023
\brief

	The file includes necessary function definition for Fuzzy Logic

Copyright (C) 2022 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents
without the prior written consent of DigiPen Institute of
Technology is prohibited.
*******************************************************************************/
#include "functions.h"

namespace AI
{
	/*!******************************************************************************/
	/*
		@brief
			overrided calculateDOM function 

		@param val
			value

		@return
			degree of membership for a particular value
	*/
	/********************************************************************************/
	float FuzzySet_LeftShoulder::calculateDOM(float val) const {
		
		// Test for the case where the left or right offsets are zero
		// (to prevent divide by zero errors below)
		if (isEqual(peakPoint, val) &&
			(isEqual(leftOffset, 0.0) || isEqual(rightOffset, 0.0)))
			return 1.0;

		// Find DOM if right of center
		if ((val >= peakPoint) && (val < (peakPoint + rightOffset)))
			return (1.0f / -rightOffset) * (val - peakPoint) + 1.0f;


		// Find DOM if left of center
		if ((val < peakPoint) && (val >= peakPoint - leftOffset))
			return 1.0;

		// Out of range
		return 0.0;
	}

	/*!******************************************************************************/
	/*
		@brief
			overrided calculateDOM function

		@param val
			value

		@return
			degree of membership for a particular value
	*/
	/********************************************************************************/
	float FuzzySet_RightShoulder::calculateDOM(float val) const
	{
		// Test for the case where the left or right offsets are zero
		// (to prevent divide by zero errors below)
		if (isEqual(peakPoint, val) &&
			(isEqual(leftOffset, 0.0) || isEqual(rightOffset, 0.0)))
			return 1.0;

		// Find DOM if left of center
		if ((val <= peakPoint) && (val > (peakPoint - leftOffset)))
			return (1.0f / leftOffset) * (val - (peakPoint - leftOffset));

		// Find DOM if right of center and less than center + right offset
		if ((val > peakPoint) && (val <= peakPoint + rightOffset))
			return 1.0;

		// Out of range
		return 0.0;
	}

	/*!******************************************************************************/
	/*
		@brief
			overrided calculateDOM function

		@param val
			value

		@return
			degree of membership for a particular value
	*/
	/********************************************************************************/
	float FuzzySet_Singleton::calculateDOM(float val) const
	{
		if ((val >= peakPoint - leftOffset)
			&& (val <= peakPoint + rightOffset))
			return 1.0;

		// Out of range
		return 0.0;
	}

	/*!******************************************************************************/
	/*
		@brief
			overrided calculateDOM function

		@param val
			value

		@return
			degree of membership for a particular value
	*/
	/********************************************************************************/
	float FuzzySet_Triangle::calculateDOM(float val) const
	{
		// Test for the case where the left or right offsets are zero
		// (to prevent divide by zero errors below)
		if (isEqual(peakPoint, val) &&
			(isEqual(leftOffset, 0.0) || isEqual(rightOffset, 0.0)))
			return 1.0;

		// Find DOM if left of center
		if ((val <= peakPoint) && (val >= (peakPoint - leftOffset)))
			return (1.0f / leftOffset) * (val - (peakPoint - leftOffset));

		// Find DOM if right of center
		if ((val > peakPoint) && (val < (peakPoint + rightOffset)))
			return (1.0f / -rightOffset) * (val - peakPoint) + 1.0f;

		// Out of range
		return 0.0;
	}

	/*!******************************************************************************/
	/*
		@brief
			overrided getDOM() function

		@return
			Returns the minimum DOM of the sets it is operating on
	*/
	/********************************************************************************/
	float FuzzyAND::getDOM()
	{
		float smallest = std::numeric_limits<float>::max();
		for (const auto& set : this->sets)
		{
			float cur = set->getDOM();
			if (cur < smallest)
				smallest = cur;
		}
		return smallest == std::numeric_limits<float>::max() ? 0.0f : smallest;
	}

	/*!******************************************************************************/
	/*
		@brief
			overrided getDOM() function

		@return
			Returns the minimum DOM of the sets it is operating on
	*/
	/********************************************************************************/
	float FuzzyOR::getDOM()
	{
		float largest = std::numeric_limits<float>::min();
		for (const auto& set : this->sets)
		{
			float cur = set->getDOM();
			if (cur > largest)
				largest = cur;
		}
		return largest == std::numeric_limits<float>::min() ? 0.0f : largest;
	}

	/*!******************************************************************************/
	/*
		@brief
			Defuzzifies the value by averaging the maxima of the sets that have fired.
			
		@return
			Returns sum (maxima * degree of membership) / sum (degree of memberships)
	*/
	/********************************************************************************/
	float FuzzyVariable::deFuzzifyMaxAv()
	{
		float bottom = 0.0;
		float top = 0.0;

		for (auto it = this->sets.begin(); it != this->sets.end(); ++it)
		{
			bottom += it->second->getDOM();
			top += it->second->getRepresentativeValue() * it->second->getDOM();
		}

		// Make sure bottom is not equal to zero
		if (isEqual(0, bottom))
			return 0.0;

		return top / bottom;
	}

	/*!******************************************************************************/
	/*
		@brief
			Defuzzify the variable using the centroid method

		@param numSamples
			number of sameple

		@return
			Dividing the total area by the sum of the moments gives the centroid
	*/
	/********************************************************************************/
	float FuzzyVariable::deFuzzifyCentroid(int numSamples)
	{
		// Calculate the step size
		float stepSize = (maxRange - minRange) / numSamples;

		float totalArea = 0.0;
		float sumOfMoments = 0.0;

		// Step through the range of this variable in increments equal to stepSize
		// adding up the contribution (lower of calculateDOM 
		// or the actual degree of membership of this
		// variable's fuzzified value) for each subset. This gives an approximation of
		// the total area of the fuzzy manifold.(This is similar to how the area under
		// a curve is calculated using calculus... the heights of lots of 'slices' are
		// summed to give the total area.)
		// In addition the moment of each slice is calculated and summed. Dividing
		// the total area by the sum of the moments gives the centroid. (Just like
		// calculating the center of mass of an object)
		for (float samp = 1; samp <= numSamples; ++samp)
		{
			// for each set get the contribution to the area. This is the lower of the 
			// value returned from calculateDOM or 
			// the actual degree of membership of the fuzzified value itself   
			for (auto set : sets)
			{
				float contribution = std::min(
					set.second->calculateDOM(minRange + samp * stepSize),
					set.second->getDOM());

				totalArea += contribution;

				sumOfMoments += (minRange + samp * stepSize) * contribution;
			}
		}

		// Make sure total area is not equal to zero
		if (isEqual(0, totalArea))
			return 0.0;

		return sumOfMoments / totalArea;
	}

	/*!******************************************************************************/
	/*
		@brief
			Given a fuzzy variable and a deffuzification method 

		@param varName
			variable name

		@param method
			method type

		@return
			crisp value
	*/
	/********************************************************************************/
	float FuzzyModule::deFuzzify(const std::string& varName, DefuzzifyMethod method)
	{
		// Clear the DOMs of all the consequents of all the rules
		setConfidencesOfConsequentsToZero();

		// Process the rules
		for (auto rule : rules)
			rule.calculate();

		// Defuzzify the resultant conclusion using the specified method
		switch (method) {
		case FuzzyModule::DefuzzifyMethod::centroid:
			return variables[varName].deFuzzifyCentroid(numSamples);
		case FuzzyModule::DefuzzifyMethod::max_av:
			return variables[varName].deFuzzifyMaxAv();
		}
		return 0.0f;
	}

} // end namespace