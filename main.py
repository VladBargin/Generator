import os
from PIL import Image, ImageDraw, ImageFont

header, footer = None, None


# Combine image `a` and image `b` to match width, fill blank space with `c`
def comb(a, b, c = (255, 255, 255)):
    new_img = Image.new('RGB', (max(a.size[0], b.size[0]), a.size[1] + b.size[1]), c)
    new_img.paste(a, (0, 0))
    new_img.paste(b, (0, a.size[1]))
    return new_img

# Create header image
def gen_header():     
    global header
    header = Image.new('RGB', (10, 10), (255, 255, 255))

# Create footer image
def gen_footer():
    global footer
    footer = Image.new('RGB', (10, 10), (255, 255, 255))
    
def fill(img, x, y, w, c = "#000000"):
    img.rectangle([(x, y), (x + w - 1), (y + w - 1)], fill=c)
    
# Creates and returns a picture for the problem
def gen_img(prob):
    img = Image.new('RGB', (m*cell_w + 2*cell_b, n*cell_w + 2*cell_b + 100), (255, 255, 255))
    img1 = ImageDraw.Draw(img)
    for i in range(n):
        for j in range(m):
            fill(img1, i*cell_w - cell_b, j*cell_w - cell_b, cell_w + cell_b * 2, "#222222")
            if prob[0][i][j]:
                col = "#888888"
            else:
                col = "#ffffff"
            fill(img1, i*cell_w + cell_b, j*cell_w + cell_b, cell_w - cell_b * 2, col)

    ey = n*cell_w + 2*cell_b
    # font = ImageFont.truetype(<font-file>, <font-size>)
    font = ImageFont.truetype("arial.ttf", 50)
    # draw.text((x, y),"Sample Text",(r,g,b))
    img1.text((0, ey),prob[1],(0,0,0), font=font)
    return img

# Returns the header, problem and footer images combined into one
# `img` is the image for the problem
def combine_img(img):
    return comb(comb(header, img), footer)

# Generate all problems
probs = []
def gen_probs():
    global probs
    os.system('./gen <.config> .out')
    
    with open('.out') as f:
        while True:
            field, solution = [], []
            inp = f.readline().rstrip()
            if inp == "-1":
                break
            field.append(list(map(int, inp.split())))
            for i in range(n - 1):
                field.append(list(map(int, f.readline().split())))
            path = f.readline().rstrip()
            for i in range(cnt_sol):
                solution.append(list(map(int, f.readline().split())))
            probs.append((field, path, solution))

def get_prob(i):
    return probs[i]


# Writes the created image and the solution to the problem to disk.
# If 'name' is empty, finds first unused number
def write_prob(sol, img, name=''):
    with open('ans.txt', 'a') as f:
        if name == '':
            names = set(os.listdir())
            i = 1
            while str(i) + '.jpg' in names:
                i += 1
            name = str(i)
        f.write(name + ':\n')
        for s in sol:
            f.write(' '.join(map(str, s)) + '\n')
        name = name + '.jpg'
        img.save(name)
    
    
# Generate one problem, create an image and write it to disk with the solution
def generate(i):
    prob = get_prob(i)
    img = combine_img(gen_img(prob))
    os.chdir(path)
    write_prob(prob[2], img)
    os.chdir(pcwd)

def test_generate(i):
    prob = get_prob(i)
    print('#' * (m + 2))
    for x in prob[0]:
        print('#', end='')
        for y in x:
            if y == 1:
                print('o', end='')
            else:
                print('.', end='')
                
        print('#')
    print('#' * (m + 2))
    
    print(prob[1])
    for p in prob[2]:
        print(p[0], p[1], '->', p[2], p[3], ':', p[4] + 1)
    print()

gen_header()
gen_footer()

path = '.'
with open(".config") as f:
    n, m = map(int, f.readline().split())
    pathl = int(f.readline())
    mi_segl = int(f.readline())
    ma_segl = int(f.readline())
    cnt_sol = int(f.readline())
    cnt_turns = int(f.readline())
    allow_abrupt_turns = int(f.readline())
    reps = int(f.readline())
    path = f.readline().rstrip()
    cell_w, cell_b = map(int, f.readline().split())
    text_font = f.readline()
    font_size = int(f.readline())

gen_probs()
print("Wanted", reps, "problems")
reps = len(probs)
print("Got", reps, "problems")

pcwd = os.getcwd()

for _ in range(reps):
    generate(_)
