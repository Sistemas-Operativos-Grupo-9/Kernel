import requests
from csnake import CodeWriter, Variable
from concurrent.futures import ThreadPoolExecutor
from tqdm import tqdm
from tqdm.contrib.concurrent import process_map


def getSudoku(i):
    return requests.get(
        "https://sugoku.herokuapp.com/board?difficulty=easy").json()['board']


sudokus = list(process_map(getSudoku, range(128), max_workers=4))
print(sudokus)

cw = CodeWriter()
cw.add('#pragma once')
cw.include('<stdint.h>')
cw.add_variable_initialization(
    Variable('sudokus', primitive=f'static uint8_t', value=sudokus))

with open(f'sudokus.h', 'w') as header:
    header.write(cw.code)
