# 和别的库处理不一样的地方和抉择

1、使用LT还是ET
   使用LT要注意busy loop的问题
   使用ET要注意丢事件的问题
   
2、listen fd使用水平触发模式 保证不丢连接 因为listen fd只有读事件没有写事件

3、LT的写事件要注意在socket写之前才开启写事件监听 写完后关闭写事件监听 否则会一直触发可写事件

4、实现小目标为支持异步http的client和server