/*
* cvwrapper.h
*
*  Created on: Dec 17, 2016
*      Author: nullifiedcat
*/

#ifndef CVWRAPPER_H_
#define CVWRAPPER_H_

class ConVar;

#include "sdk.h"
#include <string>
#include <vector>
#include <functional>

typedef void(*FnCommandCallback_t)(const CCommand &command);

// Catvar types
enum CatVar_t {
	CV_SWITCH,
	CV_INT,
	CV_FLOAT,
	CV_STRING,
	CV_ENUM,
	CV_KEY
};

// TODO reverse
// Enum Something
class CatEnum {
public:
	CatEnum(std::vector<std::string> values, int min = 0);
	std::string Name(int value);
public:
	const std::vector<std::string> value_names;
	int min_value;
	int max_value;
	int size;
};

typedef void(*FnCommandCallbackVoid_t)();

// TODO reverse, no idea how catcommands are handled
class CatCommand {
public:
	CatCommand(std::string name, std::string help, FnCommandCallback_t callback);
	CatCommand(std::string name, std::string help, FnCommandCallbackVoid_t callback);

	void Register();
public:
	const std::string name;
	const std::string help{ "" };

	FnCommandCallback_t callback{ nullptr };
	FnCommandCallbackVoid_t callback_void{ nullptr };

	ConCommand* cmd{ nullptr };
};



class CatVar {
public: // TODo, unknown reverse

	CatVar(CatVar_t type, std::string name, std::string defaults, std::string desc_short, std::string desc_long = "no description");
	CatVar(CatVar_t type, std::string name, std::string defaults, std::string desc_short, std::string desc_long, float max_val);
	CatVar(CatVar_t type, std::string name, std::string defaults, std::string desc_short, std::string desc_long, float min_val, float max_val);
	CatVar(CatEnum& cat_enum, std::string name, std::string defaults, std::string desc_short, std::string desc_long);

	inline operator bool() const { return !!convar_parent->m_Value.m_nValue; }
	inline operator int() const { return convar_parent->m_Value.m_nValue; }
	inline operator float() const { return convar_parent->m_Value.m_fValue; }
	inline void operator =(const int& value) { convar_parent->InternalSetIntValue(value); }
	inline void operator =(const float& value) { convar_parent->InternalSetFloatValue(value); }
	inline bool operator ==(const int& value) const { return convar_parent->m_Value.m_nValue == value; }
	inline bool operator ==(const float& value) const { return convar_parent->m_Value.m_fValue == value; }

	void Register();
	typedef std::function<void(CatVar*)> RegisterCallbackFn;
	void OnRegister(RegisterCallbackFn fn);
	void InstallChangeCallback(FnChangeCallback_t callback);

	[[deprecated]]
	inline bool GetBool() const { return this->operator bool(); }
	[[deprecated]]
	inline int GetInt() const { return this->operator int(); }
	[[deprecated]]
	inline float GetFloat() const { return this->operator float(); };
	inline const char* GetString() const { return convar_parent->GetString(); }
	[[deprecated]]
	inline void SetValue(float value) { this->operator =(value); }
	inline void SetValue(std::string value) { convar_parent->SetValue(value.c_str()); }
	[[deprecated]]
	inline void SetValue(int value) { this->operator =(value); }

	inline bool KeyDown() {
		return gInts.InputSys->IsButtonDown(static_cast<ButtonCode_t>(static_cast<int>(*this)));
	}

	inline const std::string& GetBase() const {
		return current_base;
	}

	// Storage for the catvar
public:
	const CatVar_t type;
	const std::string name;
	const std::string defaults{ "0" };
	const std::string desc_short{ "" };
	const std::string desc_long{ "" };
	const CatEnum* const enum_type{ nullptr };

	std::string current_base{ "0" };

	bool restricted{ false };
	float min{ 0.0f };
	float max{ 0.0f };

	std::vector<RegisterCallbackFn> callbacks{};
	bool registered{ false };

	ConVar* convar{ nullptr };
	ConVar* convar_parent{ nullptr };

	int id{ 0 };
	static int last_id;
};

// TODO, find out what this formatting does "std::vector<ClassName*?>& ArrayName()?"

// needed for registration
std::vector<CatVar*>& registrationArray();
std::vector<CatCommand*>& commandRegistrationArray();
// List to store catvars
std::vector<CatVar*>& CatVarList();

// Setup commands probs needed at int
void RegisterCatCommands();
void RegisterCatVars();
int GetRebasedCatVarCount();

std::vector<ConVar*>& RegisteredVarsList();
std::vector<ConCommand*>& RegisteredCommandsList();

#endif /* CVWRAPPER_H_ */
