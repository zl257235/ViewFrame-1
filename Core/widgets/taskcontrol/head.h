﻿/*!
 *  @brief     任务控制协议
 *  @details
 *  @author    wey
 *  @version   1.0
 *  @date      2018.09.11
 *  @warning
 *  @copyright NanJing RenGu.
 *  @note
 */
#ifndef TASK_CONTROL_HEAD_H
#define TASK_CONTROL_HEAD_H

#include <QString>
#include <QList>
#include <QDateTime>

#define LABEL_MIN_WIDTH 120
#define LABEL_MIN_HEIGHT 25

//编辑框、combobox的固定大小
#define LINEDIT_FIXED_WIDTH 173
#define LINEDIT_FIXED_HEIGHT 25

namespace TaskControlModel {

/**
 * @brief The operationalOrder enum
 */
enum Order{
    ADD,
    DELETE,
    REVISE
};

enum Type{
    Band = 0,           /*!< 频段控制 */
    State,              /*!< 状态控制 */
    Gather,             /*!< 采集控制 */
    SelfCheck,          /*!< 自检控制 */
    Instrument,         /*!< 仪器控制 */
    Turntable,          /*!< 转台控制 */
    PlayBack            /*!< 回放控制 */
};

/*!
 *  @brief 表头字段
 */
enum TaskHead{
    T_No = 0,                /*!< 序号 */
    T_TYPE,                 /*!< 任务类型 */
    T_PARAMETERS,           /*!< 任务参数 */
    T_E_TIME,               /*!< 任务执行时间 */
    T_E_TIME_LONG           /*!< 任务执行时长 */
};

/*!
 *  @brief 基本任务信息
 *  @details 作为所有任务的抽象类，包含通用的基本信息
 */
struct TaskInfo
{
    Type taskType;                  /*!< 任务类型 */
    QString parameter;              /*!< 任务参数 */
    QDateTime excuteTime;           /*!< 任务执行时间 */
    long lastTime;                  /*!< 任务执行时长 */

    virtual void dispatch() = 0;
};

typedef QList<TaskInfo *> TaskInfoList;

/*!
 *  @brief 频段控制
 */
struct BandControl: public TaskInfo
{
    BandControl(){
        taskType = Band;
    }
    double originFrequency;         /*!< 起始频率(HMz) */
    double stopFrequency;           /*!< 终止频率(HMz) */
    double frequencyStopping;       /*!< 频率步进(HMz) */

    void dispatch(){}
};

/*!
 *  @brief 状态控制
 */
struct StateControl  : public TaskInfo
{
    StateControl ()
    {
        taskType = State;
        workCyclesNumber = 1;
    }

    char workmode;                   /*!< 工作模式 0自检  1频域搜索  2驻留  3空域搜索  4中频采集  5敌我1030  6敌我1090 */
    char workCycles;                 /*!< 工作周期(50ms)   最大1000ms (20) */
    short workCyclesNumber;          /*!< 工作周期数  默认为1 */
    char workingBandwidth;           /*!< 工作带宽  0:400M  1:200M  2：40M  3:10M  4：5M */
    char antennaSelection1;          /*!< 天线选择1  0前  1后  2左  3右 */
    char antennaSelection2;          /*!< 天线选择2  0前  1后  2左  3右 */
    float attenuationCode1;          /*!< 衰减码1(dB) */
    float attenuationCode2;          /*!< 衰减码2(dB) */
    float attenuationCode3;          /*!< 衰减码3(dB) */
    float attenuationCode4;          /*!< 衰减码4(dB) */
    float attenuationCode5;          /*!< 衰减码5(dB) */
    float attenuationCode6;          /*!< 衰减码6(dB) */
    float attenuationCode7;          /*!< 衰减码7(dB) */
    float attenuationCode8;          /*!< 衰减码8(dB) */
    float attenuationCode9;          /*!< 衰减码9(dB) */
    float attenuationCode10;         /*!< 衰减码10(dB) */

    void dispatch(){}
};

/*!
 *  @brief 采集控制
 */
struct GatherControl  : public TaskInfo
{
    GatherControl ()
    {
        taskType = Gather;
        extractSwitch = false;
    }
    char gatherWay;                 /*!< 采集方式  0：DDR  1:SATA */
    char gatherType;                /*!< 采集类型  0：盲采  1:Vp采  2：Vp触发盲采 */
    char sendingPause;              /*!< 采集指令(只对SATA有效 0停止 1开始采集 2开始读取 3检索 4读取暂停 5发送暂停 其他：无效 */
    bool extractSwitch;             /*!< 抽取开关  0：不抽取(默认)  1：抽取 */
    float gatherTime;               /*!< 采集时长(ms) */
    int gatherImpulseNumber;        /*!< 采集脉冲个数  MAX:1000000 */
    float gatherFrequencyMin;       /*!< 采集频率下限(MHz) */
    float gatherFrequencyMax;       /*!< 采集频率上限(MHz) */
    float gatherPulseyMin;          /*!< 采集脉宽下限(us) */
    float gatherPulseyMax;          /*!< 采集脉宽上限(us) */
    float gatherPowerMin;           /*!< 采集功率下限(dBm) */
    float gatherPowerMax;           /*!< 采集功率上限(dBm) */
    char gatherTacticsSwitch;       /*!< 采集策略开关 Bit0:0时长无效，1时长有效(默认1)
                                                      Bit1:0个数无效，1个数有效(默认1)
                                                      Bit2:0频率无效，1频率有效(默认0)
                                                      Bit3:0脉宽无效，1脉宽有效(默认0)
                                                      Bit4:0功率无效，1功率有效(默认0) */
    void dispatch(){}
};

/*!
 *  @brief 自检控制
 */
struct SelfCheckControl  : public TaskInfo
{
    SelfCheckControl ()
    {
        taskType = SelfCheck;
        workCyclesNumber = 1;
    }
    char workmode;                   /*!< 工作模式 0自检  1工作 */
    char workCycles;                 /*!< 工作周期(50ms)   最大1000ms (20) */
    short workCyclesNumber;          /*!< 工作周期数  默认为1 */
    char workingBandwidth;           /*!< 工作带宽  0:400M  1:200M  2：40M  3:10M  4：5M */
    double originFrequency;          /*!< 起始频率(MHz) */
    double stopFrequency;            /*!< 终止频率(MHz) */
    double frequencyStopping;        /*!< 频率步进(MHz) */
    float attenuationCode1;          /*!< 衰减码1(dB) */
    float attenuationCode2;          /*!< 衰减码2(dB) */
    float attenuationCode3;          /*!< 衰减码3(dB) */
    float attenuationCode4;          /*!< 衰减码4(dB) */
    float attenuationCode5;          /*!< 衰减码5(dB) */
    float attenuationCode6;          /*!< 衰减码6(dB) */
    float attenuationCode7;          /*!< 衰减码7(dB) */
    float attenuationCode8;          /*!< 衰减码8(dB) */
    float attenuationCode9;          /*!< 衰减码9(dB) */
    float attenuationCode10;         /*!< 衰减码10(dB) */

    void dispatch(){}
};

/*!
 *  @brief 仪器控制
 */
struct InstrumentControl  : public TaskInfo
{
    InstrumentControl ()
    {
        taskType = Instrument;
    }
    short instrumentType;            /*!< 仪器类型  0：信号源  1：频谱仪  2：以上待定 */
    short instrumentModel;           /*!< 仪器型号  0：安捷伦  2：安立  2罗德斯瓦兹  3以上待定 */
    short instrumentCommType;        /*!< 仪器通信类型   0：网络  1：GPIB */
    char IPAddress[64];              /*!< 仪器IP地址  字符串 */
    short networkPort;               /*!< 网络端口 */
    short GPIBPort;                  /*!< GPIB端口 */
    double signalCarrierFrequency;   /*!< 信号载频(MHz) */
    double signalRepetitionPeriod;   /*!< 信号重复周期(us) */
    double signalPulseWidth;         /*!< 信号脉宽(us) */
    double signalPower;              /*!< 信号功率(dBm) */
    int radioFrequencySwitchControl; /*!< 射频开关控制  0关  1开 */
    int intrapulseSwitchControl;     /*!< 脉内调制开关控制  0关  1开 */
    int ImpulseSwitchControl;        /*!< 脉冲开关控制  0关  1开 */
    double centreFrequency;          /*!< 中心频率(Mhz) */
    double displayBandwidth;         /*!< 显示带宽(Mhz) */
    double IFBandwidth;              /*!< 中频带宽(Mhz) */
    double videoBandwidth;           /*!< 视频带宽(Mhz) */
    int scanTime;                    /*!< 扫描时间(ms) */
    int scanPoints;                  /*!< 扫描点数 */
    int ifReadspectrumData;          /*!< 是否读取频谱数据  0否  1是 */
    double signalBase;               /*!< 信号基底(dBm) */
    int powerCalibrationControl;     /*!< 功率校准控制  0：关  1：开 */
    int immediatePowerCalibration;   /*!< 功率立即校准  0：关  1：开 */

    void dispatch(){}
};

/*!
 *  @brief 转台控制
 */
struct TurntableControl  : public TaskInfo
{
    TurntableControl ()
    {
        taskType = Turntable;
    }
    short type;                 /*!< 转台类型  0：类型1  1：类型2... */
    double position;            /*!< 转台位置(度_0-360) */
    double speed;               /*!< 转台速度(度/秒) */
    short directionRotation;    /*!< 转台旋转方向 0顺时针  1逆时针 */
    short zeroPosSetting;       /*!< 转台零位设置  0不设置  1设置 */

    void dispatch(){}
};

/*!
 *  @brief 回放控制
 */
struct PlayBackControl  : public TaskInfo
{
    PlayBackControl()
    {
        taskType = PlayBack;
    }
    char path[256];             /*!< 回放文件路径 */
    int type;                   /*!< 回放类型  0辐射源 1全脉冲 2中频采集 3频谱 4设备状态 */
    double originPos;           /*!< 回放起始位置 0~1，表示相对整个文件的比例 */
    int speed;                  /*!< 回放速度(帧/分) */

    void dispatch(){}
};


/**
 * @brief 存储下发任务
 */
struct pendingTaskState
{
    bool ifExists;              /*!< 是否加入下发队列 */
    bool issued;                /*!< 是否已经下发 */
    Type type;                  /*!< 下发任务类型 */
    QString ExcutionTime;       /*!< 下发时间 */
};

} // namespace TaskControlModel

#endif // TASK_CONTROL_HEAD_H
