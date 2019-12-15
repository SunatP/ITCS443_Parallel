# Exercise 8 Hadoop

## มีกี่ข้อ?
   5. ข้อเอง

## Hadoop คืออะไร?

Hadoop คือ ซอฟท์แวร์ประเภท Open Source ที่จัดทำขึ้นเพื่อเป็นแพลตฟอร์มในการจัดเก็บข้อมูล ซึ่งมีกรอบการทำงานเพื่อใช้ในการจัดเก็บข้อมูลและประมวลผลข้อมูลที่มีขนาดใหญ่มากๆ ที่เราเรียกกันว่า Big Data ซึ่งเจ้าตัว Hadoop เนี่ยก็สามารถปรับขยาย ยืดหยุ่น เพื่อรองรับข้อมูลที่มีจำนวนมากมายมหาศาลได้ ทั้งนี้ก็เพราะมันมีกระบวนการประมวลผลที่แข็งแกร่งมากซึ่งเป็นผลมาจากการประมวลผลข้อมูลแบบกระจายผ่านเครื่องคอมพิวเตอร์ที่ถูกจัดอยู่ในรูปแบบ Cluster อันนำไปสู่ความสามารถในการรองรับข้อมูลที่ไม่จำกัดแถมยังมีความน่าเชื่อถือสูงอีกด้วย

![Hadooooooooop](http://bigdataexperience.org/BX/wp-content/uploads/2016/12/Hadoop1.jpg)


ส่วนประกอบของ Hadoop

    1. Cluster management
    2. Storage
    3. Processing

![arch](http://www.somkiat.cc/wp-content/uploads/2015/09/Screen-Shot-2558-09-13-at-9.47.10-PM.jpg)

โครงสร้างของ Hadoop

    1. Master Server
    2. Slave Server 

![server](http://www.somkiat.cc/wp-content/uploads/2015/09/Screen-Shot-2558-09-13-at-9.48.44-PM.jpg)

Ecosystem ของ Hadoop

![ecosys](https://i2.wp.com/saixiii.com/wp-content/uploads/2017/05/Big-data-hadoop-apache.png?w=690&ssl=1)


ข้อดีของ Hadoop 

1.  ความสามารถในการรองรับการจัดเก็บข้อมูลขนาดใหญ่มากๆทุกประเภทได้อย่างรวดเร็ว –  ด้วยปริมาณข้อมูลในปัจจุบันที่เพิ่มขึ้นอย่างต่อเนื่อง โดยเฉพาะจากแหล่งอย่างโซเชี่ยลมีเดีย และ Internet of Things (IoT) คุณสมบัติข้อนี้ของ Hadoop จึงสำคัญมาก
2.  พลังแห่งการประมวลผล – ด้วยรูปแบบการประมวลผลที่รวดเร็วจากการทำงานแบบ Cluster จึงทำให้ Hadoop กลายเป็นแพลตฟอร์มที่เป็นที่นิยมอย่างกว้างขวางในปัจจุบัน
3.  มีระบบรองรับความผิดพลาด – ด้วยการทำงานแบบ Cluster เมื่อ node ใด node หนึ่งพังลง งานที่มีการทำอยู่ในระบบจะถูกส่งไปยัง node อื่นทันทีเพื่อให้เกิดความต่อเนื่อง รวมถึงระบบเองยังมีการทำข้อมูลสำรองเก็บไว้แบบอัตโนมัติหลายชุดอีกด้วย
4.  ความยืดหยุ่นในการใช้งาน – Hadoop แตกต่างจากระบบฐานข้อมูลดั้งเดิม ที่ต้องมีการแยกประเภทของข้อมูลคร่าวๆก่อนการจัดเก็บ สำหรับ Hadoop เราจะเก็บข้อมูลประเภทไหนก็ได้ มากเท่าไหร่ก็ได้ทันที โดยไม่ต้องมีการแยกประเภทล่วงหน้าแถมยังสามารถเลือกได้อีกว่าจะเอาไปใช้งานด้านใด
5.  ต้นทุนต่ำ – เพราะเป็นแพลตฟอร์มแบบ Open Source จึงสามารถนำมาใช้งานได้ฟรี!
6.  การจัดเก็บข้อมูลไปได้เรื่อยๆตามแต่เราต้องการ

![hadoop1](http://bigdataexperience.org/BX/wp-content/uploads/2016/12/Hadoop3.png)

เครื่องมือที่สามารถทำงานร่วมกันกับ Hadoop ได้

    1. Spark
    2. NOSQL
    3. Pig
    4. Samza
    5. Hive
    6. Cascading
    7. etc.  

### ข้อ 1.
Write map and reduce functions (pseudo code) to find the minimum temperature of each month from the sample input file below. The first column is the datetime in YYYYMMDDhhmm. The second column is Degree Celsius

```sql
201001010000,25.0
201001010015,24.5
201001010030,24.0
201001010045,24.0
201001010100,23.5
201001010115,23.0
…
201512312345,27.0
```

โค้ดจะเป็นแบบนี้

```sql
map(key,value):
    (datetime,temperature) = split(value,",");
    currentdate = substring(datetime,0,6);
    Emit(currentdate,temperature);

reduce(key,value):
    min = 0 , isfirst = true;
    if isfirst == true then:
        min = value[0]
    end if
    for each temperature in value :
        if temperature < min then :
            min = temperature ;
        end if
    end for
    Emit(key,min);
```


### ข้อ 2.

Repeat Question 1 but find the minimum temperature for each year.
```sql
map(key,value):
    (datetime,temperature) = split(value,",");
    currentyear = substring(datetime,0,4);
    Emit(currentdate,temperature);

reduce(key,value):
    min = 0 , isfirst = true;
    if isfirst == true then:
        min = value[0]
    end if
    for each temperature in value :
        if temperature < min then :
            min = temperature ;
        end if
    end for
    Emit(key,min);
```

### ข้อ 3. 

Given a text file below, write map and reduce functions to output the line number that each word appears in the file. For simplicity, the duplication of some output doesn’t matter (e.g. “the 2”).




```sql
INPUT TO MAP (3 lines)
------------------------
the quick brown fox
the fox ate the mouse
how now brown cow
```
```sql
OUTPUT FROM REDUCE
------------------------
ate 2
brown 1
brown 3
…
the 1
the 2
the 2
```

```sql
map(key,value):
    for each word in split(value," "):
        Emit(word,key);
    end for
reduce(key,value):
    for each word in value:
        Emit(word,key);
    end for
```

### ข้อ 4.

Draw the flow of data from Question 3, i.e. the intermediate data from mappers to reducers, and the reducers’ output. Given that there are 3 mappers and 2 reducers, and each mapper works on each input line

![1](https://raw.githubusercontent.com/SunatP/ITCS443_Parallel/master/Exercise/EX8/img/1.PNG)

Result

![2](https://raw.githubusercontent.com/SunatP/ITCS443_Parallel/master/Exercise/EX8/img/2.PNG)
### ข้อ 5.

In a network analysis job, an administrator wants to examine unusual events from a large log file
of a web server. Each event is written line by line in the log file. Each line starts with the IP address
of browser machines. Write map and reduce functions to filter in all events that contains ERROR.
And produce the number of ERROR events of each browser machine. For example, there are 2
ERROR events from 123.125.71.20. Note that the order of events in the output files doesn’t matter

```sql
INPUT TO MAP
----------------------------
1.202.218.8 - - [20/Jun/2012:19:05:12 +0200] "GET /robots.txt OK"
208.115.113.91 - - [20/Jun/2012:19:20:16 +0200] "GET /index.html OK"
123.125.71.20 - - [20/Jun/2012:19:30:40 +0200] "GET /login.html ERROR password mismatch"
123.125.71.20 - - [20/Jun/2012:19:30:50 +0200] "GET /login.html ERROR password mismatch"
220.181.108.101 - - [20/Jun/2012:19:31:01 +0200] "GET / OK"
123.125.68.79 - - [20/Jun/2012:19:53:24 +0200] "GET / OK"
178.154.210.252 - - [20/Jun/2012:19:54:10 +0200] "GET /data/test ERROR /data/test/ is forbidden"
74.125.126.103 - - [20/Jun/2012:20:15:29 +0200] "GET /icons/blank.gif OK"
74.125.126.93 - - [20/Jun/2012:20:15:32 +0200] "GET /icons/folder OK"
```
```sql
OUTPUT FROM REDUCE
---------------------------
178.154.210.252 1
123.125.71.20 2
…
```

```sql
map(key,value):
    isError = checkword("Error",value);
    if isError == true then
        ip = searchbypattern("(?:[0-9]{1,3}\.){3}[0-9]{1,3}",value);
        Emit(ip,1);
    end if

reduce(key,value):
    sum = 0;
    for each v in value:
        sum = sum + v;
    end for
    Emit(key,sum);
```