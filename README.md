# rocket




#日志事件
```
1、日志级别
2、打印到文件，支持日期命名，日志滚动
3、c格式化风格
4、线程安全
```

LogLevel:
```
Debug
Info
Error
```

LogEvent:
```
行号
文件名
MsgNo
ThreadId
日期、时间精确到ms
自定义消息
```

日志格式
```
[level][%y-%m-%d %H:%M:%s.%ms]\t[pid:thread_id]\t[file:line]\t[msg]
```
Logger日志器
1、提供打印日志的方法
2、设置日志输出的路径

####定时任务
```
指定的时间点
到达下一次执行时间的间隔
是否为周期性的任务
取消标志
取消重复
```