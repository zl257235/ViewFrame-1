/*!
 *  @brief     控制信息结构体
 *  @details
 *  @author    wey
 *  @version   1.0
 *  @date      2018.09.11
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef PROTOCOLBASE_H
#define PROTOCOLBASE_H

#include "head.h"
#include <QList>

namespace TaskControlModel{

class Task {
public:
    explicit Task(){}
    virtual ~Task(){}

	template<class T>
	T * getControlClass() {
		return  dynamic_cast<T*>(this);
	}

	virtual void dispatch() = 0;
};

typedef QList<Task *> TaskList;

/**
 * @brief 频段控制
 */
class BandTask : public Task
{
public:
    BandTask(BandControl &info){this->info = info;}
	~BandTask() = default;

	void dispatch();

    BandControl getTaskControlData(){return this->info;}
    void setTaskControlInfo(BandControl &info){this->info = info;}

private:
    BandControl info;
};

/**
 * @brief 状态控制
 */
class StateTask : public Task
{
public:
    StateTask(StateControl & info){this->info = info;}
	~StateTask() = default;

	void dispatch();

    StateControl getTaskControlData(){return this->info;}
    void setTaskControlInfo(StateControl &info){this->info = info;}

private:
    StateControl info;
};

/**
 * @brief 采集控制
 */
class GatherTask : public Task
{
public:
    GatherTask(GatherControl & info){this->info = info;}
	~GatherTask() = default;

	void dispatch();

    GatherControl getTaskControlData(){return this->info;}
    void setTaskControlInfo(GatherControl & info){this->info = info;}

private:
    GatherControl info;
};


/**
 * @brief The SelfCheckControl class
 * @name 自检控制
 */
class SelfCheckTask : public Task
{
public:
    SelfCheckTask(SelfCheckControl & info){this->info = info;}
	~SelfCheckTask() = default;

	void dispatch();

    SelfCheckControl getTaskControlData(){return this->info;}
    void setTaskControlInfo(SelfCheckControl info){this->info = info;}

private:
    SelfCheckControl info;
};

/**
 * @brief The InstrumentControl class
 * @name 仪器控制
**/
class InstrumentTask : public Task
{
public:
    InstrumentTask(InstrumentControl &info){this->info = info;}
	~InstrumentTask() = default;

	void dispatch();

    InstrumentControl getTaskControlData(){return this->info;}
    void setTaskControlInfo(InstrumentControl& info){this->info = info;}

private:
    InstrumentControl info;
};

/**
 * @brief The TurntableTask class
 * @name 转台控制
**/
class TurntableTask : public Task
{
public:
    TurntableTask(TurntableControl & info){this->info = info;}
	~TurntableTask() = default;

	void dispatch();

    TurntableControl getTaskControlData(){return this->info;}
    void setTaskControlInfo(TurntableControl& info){this->info = info;}

private:
    TurntableControl info;
};

/**
 * @brief The PlayBackControl class
 * @name 回放控制
**/
class PlayBackTask : public Task
{
public:
    PlayBackTask(PlayBackControl & info){this->info = info;}
	~PlayBackTask() = default;

	void dispatch();

    PlayBackControl getTaskControlData(){return this->info;}
    void setTaskControlInfo(PlayBackControl & info){this->info = info;}

private:
    PlayBackControl info;
};

}

#endif  //PROTOCOLBASE_H
