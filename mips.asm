#final lab
#Jesse Osiecki
#mips.asm

#this generates random text based of markov chain probablilities. This is different than the c program in that rather than using words as tokens, it uses letters as tokens.
#using letters rather than words simplifies the nasty mips text handling, but still shows proof of concept
.data 0x0
	strn:		.asciiz "To Sherlock Holmes she is always the woman. I have seldom heard him mention her under any other name. In his eyes she eclipses and predominates the whole of her sex. It was not that he felt any emotion akin to love for Irene Adler. All emotions, and that one particularly, were abhorrent to his cold, precise but admirably balanced mind. He was, I take it, the most perfect reasoning and observing machine that the world has seen, but as a lover he would have placed himself in a false position. He never spoke of the softer passions, save with a gibe and a sneer. They were admirable things for the observer--excellent for drawing the veil from men's motives and actions. But for the trained reasoner to admit such intrusions into his own delicate and finely adjusted temperament was to introduce a distracting factor which might throw a doubt upon all his mental results. Grit in a sensitive instrument, or a crack in one of his own high-power lenses, would not be more disturbing than a strong emotion in a nature such as his. And yet there was but one woman to him, and that woman was the late Irene Adler, of dubious and questionable memory.  I had seen little of Holmes lately. My marriage had drifted us away from each other. My own complete happiness, and the home-centred interests which rise up around the man who first finds himself master of his own establishment, were sufficient to absorb all my attention, while Holmes, who loathed every form of society with his whole Bohemian soul, remained in our lodgings in Baker Street, buried among his old books, and alternating from week to week between cocaine and ambition, the drowsiness of the drug, and the fierce energy of his own keen nature. He was still, as ever, deeply attracted by the study of crime, and occupied his immense faculties and extraordinary powers of observation in following out those clues, and clearing up those mysteries which had been abandoned as hopeless by the official police. From time to time I heard some vague account of his doings: of his summons to Odessa in the case of the Trepoff murder, of his clearing up of the singular tragedy of the Atkinson brothers at Trincomalee, and finally of the mission which he had accomplished so delicately and successfully for the reigning family of Holland. Beyond these signs of his activity, however, which I merely shared with all the readers of the daily press, I knew little of my former friend and companion.  One night--it was on the twentieth of March, 1888--I was returning from a journey to a patient (for I had now returned to civil practice), when my way led me through Baker Street. As I passed the well-remembered door, which must always be associated in my mind with my wooing, and with the dark incidents of the Study in Scarlet, I was seized with a keen desire to see Holmes again, and to know how he was employing his extraordinary powers. His rooms were brilliantly lit, and, even as I looked up, I saw his tall, spare figure pass twice in a dark silhouette against the blind. He was pacing the room swiftly, eagerly, with his head sunk upon his chest and his hands clasped behind him. To me, who knew his every mood and habit, his attitude and manner told their own story. He was at work again. He had risen out of his drug-created dreams and was hot upon the scent of some new problem. I rang the bell and was shown up to the chamber which had formerly been in part my own.  His manner was not effusive. It seldom was; but he was glad, I think, to see me. With hardly a word spoken, but with a kindly eye, he waved me to an armchair, threw across his case of cigars, and indicated a spirit case and a gasogene in the corner. Then he stood before the fire and looked me over in his singular introspective fashion."
	.align 2
	newline: 	.asciiz "\n"
	.align 2
	seed:		.ascii "I"
	.align 2
	gen_len:	.word 400
.text 0x3000
main:
	lb $a0, seed($0) #set the seed as the first arg
generate:
	li $v0, 11 #char print seed. a0 should already contain char
	syscall
	addiu $t3, $0, 0 #clear t3 (counter for seed occourances)
	addiu $t0, $0, 0 #clear t0 (place marker for traversing words)
	addiu $t1, $0, 0 #clear t1 (temp store for bytes)
	move $t9, $a0 #store seed in t9
	loadbytes:
		lb $t1, strn($t0) # load byte from string
		beq $t1, $0, choosenext #if null in-char, choosenext
		addi $t0, $t0, 1 #iterate for next byte
		bne $t1, $t9, loadbytes # if the seed does not match the byte, go to the next
		addi $t3, $t3, 1 #otherwise  add 1 to $t3 to count occourances of seed
		j loadbytes #loop
	choosenext:
		li $v0, 42 #rnd int syscall
		li $a0, 0
		move $a1, $t3 #the upper boud of the random number is the # of occourances of the seed [0,upper)
		syscall #result will be in a0
		addiu $t3, $0, 0 #clear t3 (counter for seed occourances)
		addiu $t0, $0, 0 #clear t0 (place marker for traversing words)
		addiu $t1, $0, 0 #clear t1 (temp store for bytes)
	grabnext:
		lb $t1, strn($t0) #load byte from strn
		addi $t0, $t0, 1 #itterate for next byte
		bne $t1, $t9, grabnext #if not equal to seed do again
		#otherwise seed is equal to char. Check occourance # of seed
		beq $a0, $t3, gen_done #found correct occourance
		addi $t3, $t3, 1 # otherwise add one to occourance and try again
		j grabnext
	gen_done:
		lb $a0, strn($t0) # load the next byte after this desired occourance into a0
		#the byte after the desired random occourance of seed is now the new seed.
		lw $t4, gen_len
		addi $s0, $s0, 1
		blt $s0, $t4, generate
		
#######################################################################################	
exit: #the program is done	
	ori $v0, $0, 10	#System call code 10 for exit
	syscall
