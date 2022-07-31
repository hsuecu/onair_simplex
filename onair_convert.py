import readline

#from numpy import append


map ={'.': '0', '-': '1', '..': '2', '.-': '3', '-.': '4', '--': '5', '...': '6', '..-': '7', '.-.': '8', '.--': '9', '-..': 'A', '-.-': 'B', '--.': 'C', '---': 'D', '....': 'E', '...-': 'F'}
line=""
itr=0
buffer=''
while True:
    try:
        line = input()
    except:
        print('')
        break
    if line not in map:
        buffer += 'X'
    else:
        buffer+= map[line]
    if itr==1:
        print(chr(int(buffer, 16)), end='')
        itr=0
        buffer=''
    else:
        itr+=1
