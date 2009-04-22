#include <exception>
using namespace std;

/** The base exception class */
class GameException: public exception
{
public:
	GameException()
	{
		mStr = "";
	}

	GameException(const String &str)
	{
		mStr = str;
	}

	~GameException() throw(){}

	virtual const char *what() const throw()
	{
		return mStr.c_str();
	}
protected:
	String mStr;
};

/** Error initializing something */
class GameInitException: public GameException{
public:
	GameInitException(): GameException(){}
	GameInitException(const String &str): GameException(str){}
};

/** In a bad state */
class GameStateException: public GameException{
public:
	GameStateException(): GameException(){}
	GameStateException(const String &str): GameException(str){}
};

/** Configuration error */
class GameConfigException: public GameException{
public:
	GameConfigException(): GameException(){}
	GameConfigException(const String &str): GameException(str){}
};
