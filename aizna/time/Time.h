/*
 * Time.h
 *
 *  Created on: 2015年11月17日
 *      Author: jcz
 */

#pragma once

#include <time.h>
#include <sys/time.h>
#include <string>

class Time
{
public:
	// 刷新当前时间
	static void UpdateTime();

    // 时间字符串 "YYYY-MM-DD HH:MM:SS"
    static char* DateTime(void);

    // 把输入时间变换成字符串形式输出
    static char* DateTime(time_t* sec);

    // 休眠豪秒ms
    static int MsSleep(unsigned long ms);

    // 休眠纳秒us
    static int UsSleep(unsigned long us);

	// 当前时间戳(秒)
    static unsigned long GetSecond();

    // 到第二天的秒数
    static int GetSecondToNextDay();

    // 到第二天的秒数
    static int GetSecondToNextDay(time_t time1);

    // 当前时间戳(豪秒)
    static unsigned long GetMSecond();

    // 本地时间戳(秒)
    static unsigned long GetLocalSecond();

    // 本地时间戳(豪秒)
    static unsigned long GetLocalMSecond();

    /**
     * 计算两个时间点之间相差的MS数
     * @param time1  {struct timeval*}  第一个时间点
     * @param time2  {struct timeval*}  第二个时间点
     * @return {int} 相差的MS数
     */
    static unsigned int MsPass(struct timeval* time1, struct timeval* time2);

	/**
	 * 比较两个时间点
	 * @param time1  {struct timeval*}  第一个时间点
	 * @param time2  {struct timeval*}  第二个时间点
	 * @return {int}
	 * 		0: 相同时间点
	 * 		1: time1是time2的未来
	 * 		-1:time1是time2的过去
	 */
    static int CompareTimeVal(struct timeval* time1, struct timeval* time2);

    /**
     * 判断两个时间点是否跨天
     * @param time1  {time_t}  第一个时间点
	 * @param time2  {time_t}  第二个时间点
     * @return {bool} 是否跨天
     */
    static bool IsSameDay(time_t time1, time_t time2);

    /**
     * 计算src到dest之间相差的天数
     * @param time1  {time_t}  第一个时间点
     * @param time2  {time_t}  第二个时间点
     * @return {int} 相差的天数
     */
    static int DayDiff(time_t dest, time_t src);

    // 时间结构转换
    static void GetTm( time_t time1, struct tm * ptrTM);

    // 时间字符串 "YYYY-MM-DD HH:MM:SS"
    static bool StringToTime(const std::string& dateString, time_t& outTime);

private:
    static struct timeval& InitialTimeVal();
    static int InitialZoneOff();

private:
    static int              utcOff;         // 和UTC时区之前的差值
    static time_t           currentTime;
    static struct timeval   currentTimeVal;
};

