#pragma once

#include "GUITexture.h"

#define SUN_DIS 50 // fairly arbitrary - but make sure it doesn't go behind skybox

class Sun
{
	ModelTexture myTexture;	// TODO:CW don't use ModelTexture here
	Light myLight;
	vec3 myDirection;
	float myScale;
public:

	Sun(ModelTexture aTexture, float aScale)
	: myTexture(aTexture)
	, myLight(vec3(2000, 100, -200), vec3(0.988, 0.831, 0.25))
	, myScale(aScale)
	{
		if (DebugConsole* console = DebugConsole::GetInstance())
		{
			console->AddVariable("Time.DayNightSpeed", GameInfo::ourDayNightSpeed);
		}
	}

	void Update()
	{
		static float time = -3.6f;
		static float lastSpeed = 0.0f;
		float speed = (Input::IsDown(GLFW_KEY_LEFT_SHIFT) ? 10.0f : 1.0f) * GameInfo::ourDayNightSpeed;
		speed = (speed + lastSpeed * 40.0f) / 41.0f;
		lastSpeed = speed;
		time -= GameInfo::ourDeltaTime / 20.0f * speed;

		if (Input::IsPressed(GLFW_KEY_KP_ADD))
			time += 3.14f / 8.0f;
		else if (Input::IsPressed(GLFW_KEY_KP_SUBTRACT))
			time -= 3.14f / 8.0f;
		
		myLight.myPosition.y = sin(time) * 20000.0f;
		myLight.myPosition.x = cos(time) * 20000.0f;
		myLight.myPosition.z = sin(time) * 20000.0f + 10000;

		GameInfo::ourDayNightTime = 1 - (pow(1 - dot(normalize(myLight.myPosition), vec3(0, 1, 0)), 10));
		float mix1 = clamp(GameInfo::ourDayNightTime, 0.0f, 1.0f);
		float mix2 = clamp(-GameInfo::ourDayNightTime, 0.0f, 1.0f);
		myLight.myColour = mix(vec3(0.992, 0.369, 0.325), vec3(0.988, 0.988, 0.78), mix1);
		myLight.myColour = mix(myLight.myColour, vec3(0.0, 0.02, 0.1), mix2);

		// TODO:CW move
		GameInfo::ourFogColour = mix(vec3(0.5f, 0.5f, 0.5f), myLight.myColour, 0.4);
	}

	void SetDirection(vec3 aDir) { myDirection = normalize(aDir); }


	ModelTexture const& GetTexture() const	{ return myTexture; }
	vec3 GetLightDirection() const			{ return myDirection; }
	float GetScale() const					{ return myScale; }
	Light& GetLight()						{ return myLight; }
	Light const& GetLight() const			{ return myLight; }
	glm::vec3 GetColour() const				{ return myLight.myColour; }


	vec3 GetWorldPosition(vec3 aCamPos) const
	{
		vec3 sunPos = normalize(myLight.myPosition);
		sunPos *= SUN_DIS;
		return aCamPos + sunPos;
	}

};