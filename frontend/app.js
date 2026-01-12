// Sudoku Solver Frontend Application

const API_URL = 'http://localhost:8080/solve';

class SudokuApp {
    constructor() {
        this.size = 9;
        this.boxSize = 3;
        this.board = [];
        this.originalBoard = [];
        
        this.initElements();
        this.bindEvents();
        this.createBoard();
    }
    
    initElements() {
        this.boardEl = document.getElementById('board');
        this.loadingEl = document.getElementById('loading');
        this.resultEl = document.getElementById('result');
        this.sizeButtons = document.querySelectorAll('.size-btn');
        this.solveBtn = document.getElementById('solveBtn');
        this.clearBtn = document.getElementById('clearBtn');
    }
    
    bindEvents() {
        // Size selection
        this.sizeButtons.forEach(btn => {
            btn.addEventListener('click', () => {
                const newSize = parseInt(btn.dataset.size);
                if (newSize !== this.size) {
                    this.size = newSize;
                    this.boxSize = Math.sqrt(newSize);
                    this.updateSizeButtons();
                    this.createBoard();
                }
            });
        });
        
        // Solve button
        this.solveBtn.addEventListener('click', () => this.solve());
        
        // Clear button
        this.clearBtn.addEventListener('click', () => this.clear());
    }
    
    updateSizeButtons() {
        this.sizeButtons.forEach(btn => {
            btn.classList.toggle('active', parseInt(btn.dataset.size) === this.size);
        });
    }
    
    createBoard() {
        this.board = Array(this.size).fill(null).map(() => Array(this.size).fill(0));
        this.originalBoard = [];
        this.boardEl.innerHTML = '';
        this.boardEl.className = `board size-${this.size}`;
        this.boardEl.style.gridTemplateColumns = `repeat(${this.size}, var(--cell-size))`;
        
        for (let i = 0; i < this.size; i++) {
            for (let j = 0; j < this.size; j++) {
                const cell = this.createCell(i, j);
                this.boardEl.appendChild(cell);
            }
        }
        
        this.hideResult();
    }
    
    createCell(row, col) {
        const cell = document.createElement('div');
        cell.className = 'cell';
        cell.dataset.row = row;
        cell.dataset.col = col;
        
        // Add box borders
        if ((col + 1) % this.boxSize === 0 && col < this.size - 1) {
            cell.classList.add('box-right');
        }
        if ((row + 1) % this.boxSize === 0 && row < this.size - 1) {
            cell.classList.add('box-bottom');
        }
        
        const input = document.createElement('input');
        input.type = 'text';
        input.maxLength = this.size > 9 ? 2 : 1;
        input.placeholder = '';
        
        input.addEventListener('input', (e) => this.handleInput(e, row, col));
        input.addEventListener('keydown', (e) => this.handleKeydown(e, row, col));
        
        cell.appendChild(input);
        return cell;
    }
    
    handleInput(e, row, col) {
        let value = e.target.value.toUpperCase();
        
        // Validate input based on board size
        let num = 0;
        if (value === '') {
            num = 0;
        } else if (this.size <= 9) {
            // Only 1-9 for 9x9
            if (/^[1-9]$/.test(value)) {
                num = parseInt(value);
            } else {
                e.target.value = '';
                return;
            }
        } else {
            // 1-9, A-P for 16x16, A-O for 25x25
            if (/^[1-9]$/.test(value)) {
                num = parseInt(value);
            } else if (/^[A-Z]$/.test(value)) {
                num = value.charCodeAt(0) - 'A'.charCodeAt(0) + 10;
                if (num > this.size) {
                    e.target.value = '';
                    return;
                }
            } else {
                e.target.value = '';
                return;
            }
            e.target.value = value;
        }
        
        this.board[row][col] = num;
    }
    
    handleKeydown(e, row, col) {
        const inputs = this.boardEl.querySelectorAll('input');
        const index = row * this.size + col;
        
        switch(e.key) {
            case 'ArrowUp':
                if (row > 0) inputs[index - this.size].focus();
                e.preventDefault();
                break;
            case 'ArrowDown':
                if (row < this.size - 1) inputs[index + this.size].focus();
                e.preventDefault();
                break;
            case 'ArrowLeft':
                if (col > 0) inputs[index - 1].focus();
                e.preventDefault();
                break;
            case 'ArrowRight':
                if (col < this.size - 1) inputs[index + 1].focus();
                e.preventDefault();
                break;
            case 'Backspace':
                if (e.target.value === '' && col > 0) {
                    inputs[index - 1].focus();
                }
                break;
        }
    }
    
    getBoardString() {
        let str = '';
        for (let i = 0; i < this.size; i++) {
            for (let j = 0; j < this.size; j++) {
                const val = this.board[i][j];
                if (val === 0) {
                    str += '0';
                } else if (val <= 9) {
                    str += val.toString();
                } else {
                    str += String.fromCharCode('A'.charCodeAt(0) + val - 10);
                }
            }
        }
        return str;
    }
    
    async solve() {
        // Store original board
        this.originalBoard = this.board.map(row => [...row]);
        
        const boardString = this.getBoardString();
        
        // Show loading
        this.showLoading();
        this.hideResult();
        
        try {
            const response = await fetch(`${API_URL}/solve`, {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/json'
                },
                body: JSON.stringify({
                    size: this.size,
                    board: boardString
                })
            });
            
            const data = await response.json();
            
            this.hideLoading();
            
            if (data.success) {
                this.displaySolution(data.board, data.time_ms);
            } else {
                this.showResult('error', `⚠️ ${data.error}`);
            }
            
        } catch (error) {
            this.hideLoading();
            this.showResult('error', '❌ Could not connect to server. Make sure the C++ server is running on port 8080.');
            console.error('Error:', error);
        }
    }
    
    displaySolution(solutionString, timeMs) {
        const inputs = this.boardEl.querySelectorAll('input');
        const cells = this.boardEl.querySelectorAll('.cell');
        
        let delay = 0;
        for (let i = 0; i < this.size; i++) {
            for (let j = 0; j < this.size; j++) {
                const index = i * this.size + j;
                const char = solutionString[index];
                
                let val;
                if (char >= '1' && char <= '9') {
                    val = parseInt(char);
                } else if (char >= 'A' && char <= 'Z') {
                    val = char.charCodeAt(0) - 'A'.charCodeAt(0) + 10;
                } else {
                    val = 0;
                }
                
                const wasEmpty = this.originalBoard[i][j] === 0;
                const input = inputs[index];
                const cell = cells[index];
                
                if (wasEmpty && val !== 0) {
                    // Animate new values
                    setTimeout(() => {
                        if (val <= 9) {
                            input.value = val.toString();
                        } else {
                            input.value = String.fromCharCode('A'.charCodeAt(0) + val - 10);
                        }
                        cell.classList.add('solved');
                    }, delay);
                    delay += 15;
                } else {
                    cell.classList.add('fixed');
                }
                
                this.board[i][j] = val;
            }
        }
        
        setTimeout(() => {
            this.showResult('success', `✅ Solved in ${timeMs.toFixed(2)}ms`);
        }, delay);
    }
    
    clear() {
        this.createBoard();
    }
    
    showLoading() {
        this.loadingEl.classList.add('visible');
        this.solveBtn.disabled = true;
    }
    
    hideLoading() {
        this.loadingEl.classList.remove('visible');
        this.solveBtn.disabled = false;
    }
    
    showResult(type, message) {
        this.resultEl.className = `result ${type}`;
        this.resultEl.textContent = message;
    }
    
    hideResult() {
        this.resultEl.className = 'result';
        this.resultEl.textContent = '';
    }
}

// Initialize the app when DOM is ready
document.addEventListener('DOMContentLoaded', () => {
    new SudokuApp();
});
