import os, sys, random, tkinter
try:
	import pygame
except Exception:
	os.system("python -m pip install pygame")
	import pygame
	
import menu

pygame.init()
sys.setrecursionlimit(10**6)

#constants
TILE_SIZE = 20
with open("comms.txt", "r") as filename:
	lst = filename.read().split()
	#bunlar pixel değil kare sayısı cinsinden
	BOARD_WIDTH = int(lst[0])
	BOARD_HEIGHT = int(lst[1])
	MINES = int(lst[2])

#assets
tileempty = pygame.image.load(os.path.join('assets','empty.png'))
tile0 = pygame.image.load(os.path.join('assets','0.png'))
tile1 = pygame.image.load(os.path.join('assets','1.png'))
tile2 = pygame.image.load(os.path.join('assets','2.png'))
tile3 = pygame.image.load(os.path.join('assets','3.png'))
tile4 = pygame.image.load(os.path.join('assets','4.png'))
tile5 = pygame.image.load(os.path.join('assets','5.png'))
tile6 = pygame.image.load(os.path.join('assets','6.png'))
tile7 = pygame.image.load(os.path.join('assets','7.png'))
tile8 = pygame.image.load(os.path.join('assets','8.png'))
tileflag = pygame.image.load(os.path.join('assets','flag.png'))
tileredflag = pygame.image.load(os.path.join('assets','redflag.png'))
tileredmine = pygame.image.load(os.path.join('assets','minered.png'))
tilemine = pygame.image.load(os.path.join('assets','mine.png'))
explodesfx = pygame.mixer.Sound(os.path.join('assets','mine_explode.mp3'))

class Tile:
	#value negatifse mayın var
	#shown: 0->kapalı, 1->açık, 2->flag
	def __init__(self, val=0, shown=0):
		self.val = val
		self.shown = shown
	
	def display(self):
		if self.shown == 0:
			return tileempty
		elif self.shown == 2:
			return tileflag
		elif self.shown == 3:
			return tileredflag
		elif self.val<0:
			return tilemine
		else:
			if self.val == 0: return tile0
			if self.val == 1: return tile1
			if self.val == 2: return tile2
			if self.val == 3: return tile3
			if self.val == 4: return tile4
			if self.val == 5: return tile5
			if self.val == 6: return tile6
			if self.val == 7: return tile7
			if self.val == 8: return tile8

board = [[Tile(0,0) for x in range(BOARD_HEIGHT)] for y in range(BOARD_WIDTH)]
opened_tiles = MINES

#display init
screen = pygame.display.set_mode((BOARD_WIDTH*TILE_SIZE, BOARD_HEIGHT*TILE_SIZE))
for itx in range(BOARD_WIDTH):
	for ity in range(BOARD_HEIGHT):
		screen.blit(board[itx][ity].display(), (itx*TILE_SIZE, ity*TILE_SIZE))
pygame.display.flip()
pygame.display.set_caption('Minesweeper')

def mousepos():
	return (pygame.mouse.get_pos()[0]//TILE_SIZE, pygame.mouse.get_pos()[1]//TILE_SIZE)

def boardi(x,y):
	return board[x][y]

def updatescr():
	for itx in range(BOARD_WIDTH):
		for ity in range(BOARD_HEIGHT):
			screen.blit(board[itx][ity].display(), (itx*TILE_SIZE, ity*TILE_SIZE))

master = tkinter.Tk()

def buttonfu():
	master.destroy()
	pygame.quit()
	sys.exit()

def lose(x,y):
	for itx in range(BOARD_WIDTH):
		for ity in range(BOARD_HEIGHT):
			if board[itx][ity].val < 0 and board[itx][ity].shown == 0:
				board[itx][ity].shown = 1
			elif board[itx][ity].val >= 0 and board[itx][ity].shown == 2:
				board[itx][ity].shown = 3
	updatescr()
	screen.blit(tileredmine, (x*TILE_SIZE, y*TILE_SIZE))
	pygame.display.update()
	
	master.geometry('300x60')
	master.title('You lost!')
	tkinter.Label(master, text='You stepped on a mine!').pack(padx=20)
	tkinter.Button(master, text='ok', width=10, command=buttonfu).pack(pady=5)
	
	explodesfx.play()
	master.mainloop()
	
	event = pygame.event.wait()
	while event.type != pygame.QUIT:
		event = pygame.event.wait()
	pygame.quit()
	sys.exit()

def win():
	for itx in range(BOARD_WIDTH):
		for ity in range(BOARD_HEIGHT):
			if board[itx][ity].val < 0:
				board[itx][ity].shown = 2
	updatescr()
	pygame.display.update()
	
	master.geometry('300x60')
	master.title('You won!')
	tkinter.Label(master, text='You\'ve spotted every mine on the field!').pack()
	tkinter.Button(master, text='ok', width=10, command=buttonfu).pack(pady=5)
	
	master.mainloop()

def opentile(x, y, dfs=0):
	global opened_tiles
	
	if board[x][y].shown == 0:
		if board[x][y].val < 0:
			lose(x,y)
			return
		else:
			opened_tiles = opened_tiles + 1
			board[x][y].shown = 1
			if board[x][y].val == 0:
				if x>0 and y>0: opentile(x-1,y-1,1)
				if x>0: opentile(x-1,y,1)
				if x>0 and y<BOARD_HEIGHT-1: opentile(x-1,y+1,1)
				if y<BOARD_HEIGHT-1: opentile(x,y+1,1)
				if x<BOARD_WIDTH-1 and y<BOARD_HEIGHT-1: opentile(x+1,y+1,1)
				if x<BOARD_WIDTH-1: opentile(x+1,y,1)
				if x<BOARD_WIDTH-1 and y>0: opentile(x+1,y-1,1)
				if y>0: opentile(x,y-1,1)
	
	elif board[x][y].shown == 1 and dfs == 0:
		cnt=0
		if x>0 and y>0 and board[x-1][y-1].shown == 2: cnt = cnt + 1
		if x>0 and board[x-1][y].shown == 2: cnt = cnt + 1
		if x>0 and y<BOARD_HEIGHT-1 and board[x-1][y+1].shown == 2: cnt = cnt + 1
		if y<BOARD_HEIGHT-1 and board[x][y+1].shown == 2: cnt = cnt + 1
		if x<BOARD_WIDTH-1 and y<BOARD_HEIGHT-1 and board[x+1][y+1].shown == 2: cnt = cnt + 1
		if x<BOARD_WIDTH-1 and board[x+1][y].shown == 2: cnt = cnt + 1
		if x<BOARD_WIDTH-1 and y>0 and board[x+1][y-1].shown == 2: cnt = cnt + 1
		if y>0 and board[x][y-1].shown == 2: cnt = cnt + 1
		
		if cnt == board[x][y].val:
			#komşu karelerde mayın varsa önce mayın olan kareyi aç
			if x>0 and y>0 and board[x-1][y-1].val < 0: opentile(x-1, y-1)
			if x>0 and board[x-1][y].val < 0: opentile(x-1, y)
			if x>0 and y<BOARD_HEIGHT-1 and board[x-1][y+1].val < 0: opentile(x-1, y+1)
			if y<BOARD_HEIGHT-1 and board[x][y+1].val < 0: opentile(x, y+1)
			if x<BOARD_WIDTH-1 and y<BOARD_HEIGHT-1 and board[x+1][y+1].val < 0: opentile(x+1, y+1)
			if x<BOARD_WIDTH-1 and board[x+1][y].val < 0: opentile(x+1, y)
			if x<BOARD_WIDTH-1 and y>0 and board[x+1][y-1].val < 0: opentile(x+1, y-1)
			if y>0 and board[x][y-1].val < 0: opentile(x, y-1)
			
			if x>0 and y>0: opentile(x-1,y-1,1)
			if x>0: opentile(x-1,y,1)
			if x>0 and y<BOARD_HEIGHT-1: opentile(x-1,y+1,1)
			if y<BOARD_HEIGHT-1: opentile(x,y+1,1)
			if x<BOARD_WIDTH-1 and y<BOARD_HEIGHT-1: opentile(x+1,y+1,1)
			if x<BOARD_WIDTH-1: opentile(x+1,y,1)
			if x<BOARD_WIDTH-1 and y>0: opentile(x+1,y-1,1)
			if y>0: opentile(x,y-1,1)

def firstmove(x,y):
	pos = (x,y)
	
	minesrem = MINES
	tilesrem = BOARD_WIDTH * BOARD_HEIGHT
	
	if pos == (0,0) or pos == (0,BOARD_HEIGHT-1) or pos == (BOARD_WIDTH-1, 0) or pos == (BOARD_WIDTH-1, BOARD_HEIGHT-1):
		tilesrem = tilesrem - 4
	elif pos[0] == 0 or pos[0] == BOARD_WIDTH-1 or pos[1] == 0 or pos[1] == BOARD_HEIGHT-1:
		tilesrem = tilesrem - 6
	else:
		tilesrem = tilesrem - 9
	
	for itx in range(BOARD_WIDTH):
		for ity in range(BOARD_HEIGHT):
			if itx <= pos[0]+1 and itx >= pos[0]-1 and ity <= pos[1]+1 and ity >= pos[1]-1:
				continue
			
			if random.randint(0, tilesrem-1) < minesrem:
				board[itx][ity].val = board[itx][ity].val - 9
				minesrem = minesrem - 1
				
				if itx>0 and ity>0: board[itx-1][ity-1].val = board[itx-1][ity-1].val + 1
				if itx>0: board[itx-1][ity].val = board[itx-1][ity].val + 1
				if itx>0 and ity<BOARD_HEIGHT-1: board[itx-1][ity+1].val = board[itx-1][ity+1].val + 1
				if ity<BOARD_HEIGHT-1: board[itx][ity+1].val = board[itx][ity+1].val + 1
				if itx<BOARD_WIDTH-1 and ity<BOARD_HEIGHT-1: board[itx+1][ity+1].val = board[itx+1][ity+1].val + 1
				if itx<BOARD_WIDTH-1: board[itx+1][ity].val = board[itx+1][ity].val + 1
				if itx<BOARD_WIDTH-1 and ity>0: board[itx+1][ity-1].val = board[itx+1][ity-1].val + 1
				if ity>0: board[itx][ity-1].val = board[itx][ity-1].val + 1
			
			tilesrem = tilesrem - 1
	opentile(*pos)
	updatescr()
	pygame.display.update()

def main():
	global opened_tiles
	
	nomoves = 1
	run = 1
	
	while run:
		updatescr()
		pygame.display.update()
		
		for event in pygame.event.get():
			if event.type == pygame.QUIT:
				pygame.quit()
				sys.exit()
			
			if event.type == pygame.MOUSEBUTTONDOWN:
				if pygame.mouse.get_pressed(3)[0]+pygame.mouse.get_pressed(3)[1]+pygame.mouse.get_pressed(3)[2] > 1:
					continue
								
				if pygame.mouse.get_pressed(3)[2]:
					pos = mousepos()
					if boardi(*pos).shown == 0:
						boardi(*pos).shown = 2
					elif boardi(*pos).shown == 2:
						boardi(*pos).shown = 0
				
				elif pygame.mouse.get_pressed(3)[0]:
					pos = mousepos()
					if boardi(*pos).shown == 1 or boardi(*pos).shown == 2:
						opentile(*pos)
						if opened_tiles == BOARD_HEIGHT*BOARD_WIDTH:
							win()
					else:
						screen.blit(tile0, (pos[0]*TILE_SIZE, pos[1]*TILE_SIZE))
						pygame.display.update()
						while True:
							event = pygame.event.wait()
							if event.type == pygame.MOUSEBUTTONUP and pygame.mouse.get_pressed(3)[0] == 0:
								break
						if mousepos() == pos:
							if nomoves:
								firstmove(*pos)
								nomoves = 0
							else:
								opentile(*pos)
							if opened_tiles == BOARD_HEIGHT*BOARD_WIDTH:
								win()

main()
