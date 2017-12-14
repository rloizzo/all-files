import sys

combinators = set(['S','I','K','B','Y','W','C','P','T','J'])
built_ins = set(['+','-','/','*'])

#class for bianry tree to hold arithmetic expressions
class Node:
	def __init__(self, symbol, parent):
		self.sym = symbol
		self.parent = parent
		self.children = []

#process built-in arithmetic tree and computes result
def process_tree(n):
	for node in reversed(n.children): # process all the operators in the child list
		if type(node.sym) is not int:
			r = process_tree(node)
	
	# once the level of the tree is all ints
	nums = n.children
	result = nums[0].sym
	for num in nums[1:]:
		if n.sym == '+':
			result += num.sym
		elif n.sym == '-':
			result -= num.sym
		elif n.sym == '*':
			result *= num.sym
		else:
			result /= num.sym
	if n.parent == None:
		return result
	else:
		n.sym = result # replace an operator in the tree with the calculated result, so that it can be used by the next operator above it in the tree
		return

#creates tree for arithmetic to be processed
def make_arith_tree(line):
	line = list(line)
	if line[-1] == "\n": # delete newline char from end of list
		del line[-1]

	if line[0] != '(': # case where only a single number is inputted
		print("=> {}".format(line[0]))
		return None
	elif line[2] == ')': # case where single number inputted in parentheses
		print("=> {}".format(line[1]))
		return None
	else: #create root of tree starting with first operator inputted
		root = Node(line[1], None)
		curr = root

	parenthese_found = False #boolean to indicate whenever a '(' is encountered
	for sym in line[2:]: #everything after first '(' and first operator
		if sym == " ": # skip any whitespace char in list
			continue
		if sym == '(':
			parenthese_found = True
		elif sym == '+' or sym == '-' or sym == '*' or sym == '/':
			if parenthese_found: # if a '(' directly precedes operator, a new layer in the tree needs to be created
				n = Node(sym, curr)
				curr.children.append(n)
				curr = n # move down one level in the tree
				parenthese_found = False
		elif sym == ')': # end of level in tree, need to move back up 
			if not parenthese_found: # if the bool is still True, we did not find an operator after the '(', i.e. a (int) occured, so we do not want to move back up
				curr = curr.parent
			parenthese_found = False 
		else: #number found
			n = Node(int(sym), curr)
			curr.children.append(n)
	return root

# This function takes an array of terms, turns it back into a list, and then reformats in order to properly seperate terms as they move to front of list. Also works if input is already a list
def make_array(arr, final_str, pr):
	s = ""
	a = []
	counter = 0
	for arg in arr:
		if len(arg) > 1: # add parentheses around grouped arguments
			arg_str = "("
			arg_str += arg
			arg_str += ")"
			s += arg_str
		else:
			s += arg	
	if pr: #prints only on final output by default
		print("=>{}".format(final_str+s)) #final_str holds not combinators that make it to front of expression
	for i in range(len(s)):
		if i == 0 and s[i] == '(': #skip parenthese at very beginning expression
			continue 
		if s[i] == '(':
			counter += 1
			if counter == 1: #beginning of argument that needs to be grouped
				arg_start = i + 1
		elif s[i] == ')':
			counter -= 1
			if counter == 0: #end of argument that needs to be grouped
				a.append(s[arg_start:i])
			elif counter == -1:
				counter = 0
		else:
			if counter == 0: #no parentheses, add single char to array
				a.append(s[i])
	return a

#function to read in a combinator and perform the proper operation, outputting a new list
def handle_combinator(a, comb):
	new_a = []
	if comb == 'S':
		new_a.append(a[1])
		new_a.append(a[3])
		arg_str = ""
		if len(a[2]) > 1:
			arg_str += "("
			arg_str += a[2]
			arg_str += ")"
		else:
			arg_str += a[2]
		if len(a[3]) > 1:
			arg_str += "("
			arg_str += a[3]
			arg_str += ")"
		else:
			arg_str += a[3]
		new_a.append(arg_str)
		for arg in a[4:]:
			new_a.append(arg)
	elif comb == 'I':
		new_a = a
		del new_a[0]
	elif comb == 'K':
		new_a.append(a[1])
		for i in range(3,len(a)):
			new_a.append(a[i])
	elif comb == 'B':
		new_a.append(a[1])
		arg_str = ""
		if len(a[2]) > 1:
			arg_str += "("
			arg_str += a[2]
			arg_str += ")"
		else:
			arg_str += a[2]
		if len(a[3]) > 1:
			arg_str += "("
			arg_str += a[3]
			arg_str += ")"
		else:
			arg_str += a[3]
		new_a.append(arg_str)
		for arg in a[4:]:
			new_a.append(arg)
	elif comb == 'Y':
		new_a.append(a[1])
		arg_str = ""
		arg_str += "Y"
		if len(a[1]) > 1:
			arg_str += "("
			arg_str += a[1]
			arg_str += ")"
		else:
			arg_str += a[1]
		new_a.append(arg_str)
		for arg in a[2:]:
			new_a.append(arg)
	elif comb == 'W':
		new_a.append(a[1])
		new_a.append(a[2])
		new_a.append(a[2])
		for arg in a[3:]:
			new_a.append(arg)
	elif comb == 'C':
		new_a.append(a[1])
		new_a.append(a[3])
		new_a.append(a[2])
		for arg in a[4:]:
			new_a.append(arg)
	elif comb == 'P':
		new_a.append(a[3])
		new_a.append(a[2])
		new_a.append(a[1])
		for arg in a[4:]:
			new_a.append(arg)
	elif comb == 'T':
		new_a.append(a[2])
		new_a.append(a[1])
		for arg in a[3:]:
			new_a.append(arg)
	elif comb == 'J':
		new_a.append("I")
		for arg in a[2:]:
			new_a.append(arg)
	return new_a

# main function to reduce an expression
def comb_reduce(line):
	final_str = "" #holds non-combinators that make it to front of expression
	a = make_array(line, final_str,False) #turn list into array
	if a[0] in built_ins: #if expression is arithmetic
		r = make_arith_tree(line)
		print("=> {}".format(process_tree(r)))
		return
	#process combinators
	done = False
	while not done:
		if a[0] in combinators: #check if first variable is combinator
			a = handle_combinator(a,a[0])
			a = make_array(a,final_str,True) #change to False skip steps of reduction
		else:
			#non combinator appearing at front
			for i in range(len(a)):
				j = 0
				while a[i][j] == '(': #skip any parentheses that may be at front
					j += 1
				x = a[i][j] 
				if x in combinators: #if another combinator is somewhere in the expression, move everything in front to final_str so that program can reduce rest of expression
					for char in a[:i]:
						if len(char) > 1:
							final_str += "("
							final_str += char
							final_str += ")"
						else:
							final_str += char
					a = a[i:]
					break
				elif i+1 == len(a): #if end of array reached with no combinators found
					done = True
				i += 1
			a = make_array(a,final_str,False)
	#print final output
	#a = make_array(a,final_str,True) #uncomment when skipping steps of reduction
	return

if __name__ == '__main__':
	if len(sys.argv) > 1:
		input_done = False
		file_name = sys.argv[1]
		f = open(file_name)
		for line in f:
			line = line.rstrip()
			print(line)
			if line[0] == '-':
				input_done = True
			elif line[0] == '#':
				continue
			if not input_done:
				comb_reduce(line)
	else:
		while True:
			line = sys.stdin.readline().rstrip()
			if len(line):
				comb_reduce(line)
			else:
				break
