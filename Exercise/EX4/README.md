# 4. Performance Evaluation

### 1.Based on the following data decomposition strategy, fill in the table below for n = 100 and p = 7

- First element controlled by process i = [i*n/p]
- Last element controlled by process i = [(i+1)*n/p]-1
- Block size = Last - First + 1

| Process | First Element  |Last Element| Block Size|
|:---:|:---:|:---:|:---:|
|0|0|13|14|
|1|14|27|14|
|2|28|41|14|
|3|42|56|15|
|4|57|70|15|
|5|71|84|14|
|6|85|99|15|
||||Total = 100|
<br>
โจทย์กำหนดจำนวนตัวเลข n มาให้ 100 ตัว และ p หรือ process มาให้ทั้งหมด 7 ตัว (0-6)
วิธีคิดโปรเซสที่ 0
ค่าตัวแรก หรือ First Element(i) : สูตรคือ i = (i*n)/p ซึ่ง i คือ โปรเซสในตาราง
i ที่ 0 , i = (0*100)/7 จะได้ 0 
ค่าตัวสุดท้าย หรือ Last Element(i) : สูตรคือ i = [(i+1)*n/p]-1 ซึ่ง i คือ โปรเซสในตาราง
i ที่ 0 , i = [(0+1)*100/7]-1 ค่าที่ได้คือ 13.285714286 ซึ่งเราจะต้องปัดทศนิยมลงจะได้ค่าเท่ากับ 13

Block Size หรือ ช่องเมมโมรี่ สูตรคือ ค่าสุดท้าย - ค่าแรก + 1
เช่น Block Size ที่ โปรเซส 0 มีค่าเป็น 13 - 0 + 1 เท่ากับ 14
