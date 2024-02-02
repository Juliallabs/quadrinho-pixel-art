//chamar loopserver(); no loop
//chamar setupWebServer() no setup

#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include "twitch.h"
#include <ESP8266WebServer.h>
#include "iot_settings.h"
#include "display_pixel.h"



const char index_html[] PROGMEM = R"rawliteral(


<meta charset="utf-8"/>
          <link rel="stylesheet" href="bpp.css" />
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>WebSocket Example</title>
<style>
  .status-bar {
    position: fixed;
    top: 0;
    left: 0;
    width: 100%;
    height: 50px;
    background-color: red;
    color: white;
    font-size: 18px;
    text-align: center;
    line-height: 50px;
    z-index: 9999;
  }

  .connected {
    background-color: green;
    height: 10px;
  }

  .ip-input {
    display: inline-block;
    margin-left: 10px;
  }
</style>
</head>
<body>

<div id="statusBar" class="status-bar">
  <span id="statusText">Desconectado, o ip do ESP é:</span>
  <input type="text" id="ipInput" class="ip-input">
</div>

<button id="button" class="status" onclick="sendClick()">Clique Aqui</button>
<br><br>
Digite algo para enviar ao ESP:<br>
<input type="text" id="textInput" oninput="sendText(this.value)">
<br><br>
<div id="receivedText"></div>

<script>

var ipInput = document.getElementById('ipInput');
var textInput = document.getElementById('textInput');
var statusBar = document.getElementById('statusBar');
var statusText = document.getElementById('statusText');
var websocket;
var lastEnvio = "";

// Função para obter o IP salvo do armazenamento local
function getSavedIP() {
    return localStorage.getItem('savedIP');
}

// Função para salvar o IP no armazenamento local
function saveIP(ip) {
    localStorage.setItem('savedIP', ip);
}

// Função para preencher automaticamente o campo de entrada do IP com o IP salvo
function fillIPInput() {
    var savedIP = getSavedIP();
    if (savedIP) {
        ipInput.value = savedIP;
    }
}

// Função para conectar WebSocket
function connectWebSocket() {
    var ip = ipInput.value.trim();
    websocket = new WebSocket('ws://' + ip + ':81/');
    
    websocket.onopen = function(event) {
        button.classList.add('connected');
        statusBar.classList.add('connected');
        statusText.innerHTML = 'Conectado';
        ipInput.style.display = 'none';
    };
    
    websocket.onclose = function(event) {
        button.classList.remove('connected');
        statusBar.classList.remove('connected');
        statusText.innerHTML = 'Desconectado';
        ipInput.style.display = 'inline-block';
        // Tente reconectar após 3 segundos
        setTimeout(connectWebSocket, 3000);
    };
    
    websocket.onmessage = function(event) {
        console.log("Recebido do ESP: \n" + event.data);
    };
}

// Evento de escuta para conectar WebSocket quando o usuário digita no campo de entrada do IP ou pressiona Enter
ipInput.addEventListener('input', function(event) {
    connectWebSocket();
});

ipInput.addEventListener('keypress', function(event) {
    if (event.key === 'Enter') {
        connectWebSocket();
    }
});

var lastEnvioTime = Date.now();

function sendText(text) {
    if (websocket.readyState === WebSocket.OPEN) {
        var currentTime = Date.now();
        var elapsedTime = currentTime - lastEnvioTime;

        if (elapsedTime < 50) { //não enviar se o ultimo envio estiver sido abaixo deste tempo em ms
            console.log("Tempo mínimo entre envios não atendido. Ignorando o envio.");
            return; // Ignora o envio se o tempo mínimo entre envios não for atendido
        }

        var trimmedText = text ? text.trim() : null; // Verifica se o texto não é undefined
        if (!trimmedText || trimmedText === "") {
            console.log("Texto vazio ou undefined. Não enviado.");
            return; // Ignora o envio se o texto estiver vazio ou undefined
        }

        if (trimmedText !== lastEnvio) {
            websocket.send(trimmedText);
            lastEnvio = trimmedText;
            lastEnvioTime = currentTime; // Atualiza o horário do último envio
        } else {
            console.log("Texto igual ao último envio. Não enviado.");
        }
    }
}

let lastUrl = '';

function sendTextAjax(url) {
  if (url === lastUrl) {
    console.log('A URL é a mesma da chamada anterior:', url);
    return; // Ignora a requisição
  }

  lastUrl = url; // Atualiza a última URL utilizada

  fetch(url)
    .then(response => {
      if (!response.ok) {
        throw new Error('Erro ao executar a requisição');
      }
      return response.text(); // Use .json() se a resposta for JSON
    })
    .then(data => {
      // Exibe a resposta do servidor na Serial (console)
      console.log(data);
    })
    .catch(error => {
      console.error('Erro:', error);
    });
}

// Exemplo de uso
//requestAjax('http://exemplo.com/recurso?parametro=valor');


document.body.addEventListener('touchmove', function(event) {
    // Verifica se o alvo do evento é o próprio elemento body ou um descendente dele
    if (event.target === document.body || document.body.contains(event.target)) {
        // Mostra uma mensagem no console
        console.log("Tentativa de mover detectada no body!");
        
        // Previne o comportamento padrão do navegador
        event.preventDefault();
    }
});



// Preencher automaticamente o campo de entrada do IP quando a página é carregada
fillIPInput();

// Conectar WebSocket quando a página é carregada
connectWebSocket();



</script>
</body>
</html>





    
<script>
        // Verifica se a página está sendo carregada em HTTP
        if (window.location.protocol === 'http:') {
            // Redireciona para a mesma URL, mas com o protocolo HTTP
            //  window.location.href = 'https://' + window.location.host + window.location.pathname + window.location.search;
        }
</script>





<!DOCTYPE html>
<html lang="pt-br" >
<head>
  <meta charset="UTF-8">
  <title>Pixel Led Art Editor</title>
  <meta property="og:url" content="https://rerre.net/PixelEditor/">
  <meta property="og:type" content="website">
  <meta property="og:title" content="PixelEditor ✏️">
  <meta property="og:description" content="https://rerre.net/PixelEditor/">
  <meta property="og:image" content="http://rerre.net/PixelEditor/aovivo.php&png">

  <meta name="color-scheme" content="light">
  <link rel="stylesheet" href="./style.css">
  <meta name="mobile-web-app-capable" content="yes">
  <meta name="apple-mobile-web-app-capable" content="yes">
</head>
<body>
<!-- partial:index.partial.html -->
  <head>
    <meta charset="utf-8">
    <!-- Ensures proper rendering and touch zooming -->
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <!-- Forces IE 8/9/10 to use its latest rendering engine -->
    <meta http-equiv="x-ua-compatible" content="ie=edge">
    <link rel="stylesheet" href="https://fonts.googleapis.com/css?family=Helvetica">
  
  </head>
  <body>
    <div class="row">
      <div class="column">
        <div class="inputs">
          <h4>size</h4>
          <!-- class names all lower case and delimited with hyphens per Udacity Frontend Nanodegree Style Guide (CSS page) -->
          <form class="size-picker">
            height:
            <input type="number" class="input-height" id="input-height" name="height" min="1" max="128" value="8">
            <br>
            width:
            <input type="number" class="input-width" id="input-width" name="width" min="1" max="128" value="8">            
            <input type="submit" class="button" value="apply/clear">
            <p><em></em></p></form>
            scale:
            <input type="number" id="input-scale" class="input-scale" name="height" min="1" max="128" value="50" onchange="updateBoxSize(this.value)">

            <script>
        function updateBoxSize(size) {
            const boxes = document.getElementsByClassName('box');
            for (let i = 0; i < boxes.length; i++) {
                boxes[i].style.width = size + 'px';
                boxes[i].style.height = size + 'px';
            }
        }
    </script>

            <br><br>
                      <button id="divisoriasButton">Divisórias</button>
          <h4>select color</h4>
          <input type="color" class="color-picker" value="#6aa5b8">
          <div id="paleta">
fixed palette:<br>
<div class="color" style="background-color: #ff0000;" onclick="selectColor(this)"></div>
<div class="color" style="background-color: #00ff00;" onclick="selectColor(this)"></div>
<div class="color" style="background-color: #0000ff;" onclick="selectColor(this)"></div>
<div class="color" style="background-color: #ffff00;" onclick="selectColor(this)"></div>
<div class="color" style="background-color: #ff00ff;" onclick="selectColor(this)"></div>
<div class="color" style="background-color: #00ffff;" onclick="selectColor(this)"></div>
<br>palette array:<br>
</div>
          <h4>fill with color</h4>
          <input type="button" class="quick-fill" value="fill all">
          <h4>modes:</h4>
          <input type="button" class="erase-mode" value="eraser">
          <p><em>Double-click on a square will erase it.</em></p>
          <input type="button" class="draw-mode" value="pencil">
        </div>
      </div>

      <div class="column">
      <div class="titulo">        <h1 id="textoAlternando">Pixel LED Art Creator for Arduino</h1><br></div>
      <div class="tabelapixel"><table class="pixel-canvas"></table></div> 
      </div>     

    </div>
    
  </body>
</html>
<!-- partial -->
  <script>
    // Array com as palavras que queremos alternar
    const palavras = ["Pixel LED Art Creator for ESP32", "Pixel LED Art Creator for esp8266", "Pixel LED Art Creator for Arduino",];

    let index = 0; // Índice para acompanhar a palavra atual

    function alternarPalavra() {
      const texto = document.getElementById("textoAlternando");
      texto.innerText = palavras[index];

      // Incrementar o índice para a próxima palavra
      index = (index + 1) % palavras.length;
    }

    // Chamar a função para alternar a palavra a cada 1 segundo (1000 milissegundos)
    setInterval(alternarPalavra, 3210);
  </script>


</body>
</html>






<script>
  const sizePicker = document.querySelector('.size-picker');
const pixelCanvas = document.querySelector('.pixel-canvas');
const quickFill = document.querySelector('.quick-fill');
const eraseMode = document.querySelector('.erase-mode');
const drawMode = document.querySelector('.draw-mode');

// Função para desabilitar a rolagem da página
function disablePageScroll() {
  document.body.style.overflow = 'hidden';
}

// Função para habilitar a rolagem da página novamente
function enablePageScroll() {
  document.body.style.overflow = '';
}
function makeGrid() {
  let gridHeight = document.querySelector('.input-height').value;
  let gridWidth = document.querySelector('.input-width').value;
  // If grid already present, clears any cells that have been filled in
  while (pixelCanvas.firstChild) {
    pixelCanvas.removeChild(pixelCanvas.firstChild);
  }
  // Creates rows and cells
  for (let i = 0; i < gridHeight; i++) {
    let gridRow = document.createElement('tr');
    pixelCanvas.appendChild(gridRow);
    for (let j = 0; j < gridWidth; j++) {
      let gridCell = document.createElement('td');
      gridCell.classList.add('box'); // Adiciona a classe 'box' ao <td>
      gridRow.appendChild(gridCell);
      // Se for a primeira célula da primeira linha, colorir explicitamente
      if (i === 0 && j === 0) {
        gridCell.style.backgroundColor = "#000";
      }
      gridCell.style.backgroundColor = "#000";
    }
  }
}








makeGrid(8, 8);

// Upon user's submitting height and width selections, callback function (inside method) calls makeGrid function. But event method preventDefault() first intercepts the 'submit' event, which would normally submit the form and refresh the page, preventing makeGrid() from being processed
sizePicker.addEventListener('submit', function(e) {
  e.preventDefault();
  makeGrid();
  updateBoxSize(getAndPrintValueID('input-scale'));
});

// Enables color dragging with selected color (code for filling in single cell is above). (No click on 'draw' mode needed; this is default mode)
let down = false; // Tracks whether or not mouse pointer is pressed

// Listens for mouse pointer press and release on grid. Changes value to true when pressed, but sets it back to false as soon as released
pixelCanvas.addEventListener('mousedown', function(e) {
  down = true;
  pixelCanvas.addEventListener('mouseup', function() {
    down = false;
  });
  // Ensures cells won't be colored if grid is left while pointer is held down
  pixelCanvas.addEventListener('mouseleave', function() {
    down = false;
  });

  pixelCanvas.addEventListener('mouseover', function(e) {
    // 'color' defined here rather than globally so JS checks whether user has changed color with each new mouse press on cell
    const color = document.querySelector('.color-picker').value;
    // While mouse pointer is pressed and within grid boundaries, fills cell with selected color. Inner if statement fixes bug that fills in entire grid
    if (down) {
      // 'TD' capitalized because element.tagName returns upper case for DOM trees that represent HTML elements
      if (e.target.tagName === 'TD') {
        e.target.style.backgroundColor = color;
        updateColorArray();
        processarConteudoTextarea();
      }
    }
  });

  // Ativa a pintura do quadradinho ao clicar nele
pixelCanvas.addEventListener('mousedown', function(e) {
  // 'color' definida aqui em vez de globalmente para que o JS verifique se o usuário mudou a cor com cada novo clique do mouse na célula
  const color = document.querySelector('.color-picker').value;
  // Pinta o quadradinho com a cor selecionada
  if (e.target.tagName === 'TD') {
    e.target.style.backgroundColor = color;
    updateColorArray();
    processarConteudoTextarea();
  }
});

let lastTouchedCell; // Variável para armazenar a última célula tocada

pixelCanvas.addEventListener('touchstart', function(e) {
  const color = document.querySelector('.color-picker').value;
  if (e.target.tagName === 'TD') {
    // Armazena a célula onde o toque começou
    lastTouchedCell = e.target;
    e.target.style.backgroundColor = color;
    updateColorArray();
    processarConteudoTextarea();
    disablePageScroll();
  }
});

pixelCanvas.addEventListener('touchmove', function(e) {
  const color = document.querySelector('.color-picker').value;
  if (e.target.tagName === 'TD' && lastTouchedCell) {
    // Pinta a célula onde o toque começou
    lastTouchedCell.style.backgroundColor = color;
    updateColorArray();
    processarConteudoTextarea();
    disablePageScroll();

    // Percorre o caminho do toque e pinta as células consecutivas até o novo alvo
    const touchPath = e.touches[0]; // Obter a posição do toque atual
    const newTouchedCell = document.elementFromPoint(touchPath.clientX, touchPath.clientY); // Obter o elemento tocado no novo local
    if (newTouchedCell !== lastTouchedCell && newTouchedCell.tagName === 'TD') {
      newTouchedCell.style.backgroundColor = color;
      updateColorArray();
      processarConteudoTextarea();
      lastTouchedCell = newTouchedCell; // Atualiza a célula tocada anteriormente para a nova célula
    }
  }
});

pixelCanvas.addEventListener('touchend', function(e) {
  // Limpa a variável que armazena a última célula tocada
  lastTouchedCell = null;
  enablePageScroll();
});


});

// Adds color-fill functionality. e.preventDefault(); intercepts page refresh on button click
quickFill.addEventListener('click', function(e) {
  e.preventDefault();
  const color = document.querySelector('.color-picker').value;
  pixelCanvas.querySelectorAll('td').forEach(td => td.style.backgroundColor = color);
});

// Removes color from cell upon double-click
pixelCanvas.addEventListener('dblclick', e => {
  e.target.style.backgroundColor = '#000';
  updateColorArray();
});

// NONDEFAULT DRAW AND ERASE MODES:

// Allows for drag and single-cell erasing upon clicking 'erase' button. Code for double-click erase functionality (Without entering erase mode) is above. Also note 'down' was set to false in variable above
eraseMode.addEventListener('click', function() {
  // Enables drag erasing while in erase mode
  pixelCanvas.addEventListener('mousedown', function(e) {
    down = true;
    pixelCanvas.addEventListener('mouseup', function() {
      down = false;
    });
    // Ensures cells won't be erased if grid is left while pointer is held down
    pixelCanvas.addEventListener('mouseleave', function() {
      down = false;
    });
    pixelCanvas.addEventListener('mouseover', function(e) {
      // While mouse pointer is pressed and within grid boundaries, empties cell contents. Inner if statement fixes bug that fills in entire grid
      if (down) {
        if (e.target.tagName === 'TD') {
          e.target.style.backgroundColor = '#000';
        }
      }
    });
  });
  // Enables single-cell erase while in erase mode
  pixelCanvas.addEventListener('mousedown', function(e) {
    e.target.style.backgroundColor = '#000';

  });
});

// Allows user to return to (default) draw mode after using 'erase' button. Note 'down' was set to false in variable above
drawMode.addEventListener('click', function() {
  pixelCanvas.addEventListener('mousedown', function(e) {
    down = true;
    pixelCanvas.addEventListener('mouseup', function() {
      down = false;
    });
    // Ensures cells won't be colored if grid is left while pointer is held down
    pixelCanvas.addEventListener('mouseleave', function() {
      down = false;
    });
    pixelCanvas.addEventListener('mouseover', function(e) {
      const color = document.querySelector('.color-picker').value;
      // While mouse pointer is pressed and within grid boundaries, fills cell with selected color. Inner if statement fixes bug that fills in entire grid
      if (down) {
        if (e.target.tagName === 'TD') {
          e.target.style.backgroundColor = color;
        }
      }
    });
  });
  // Enables single-cell coloring while in draw mode
  pixelCanvas.addEventListener('mousedown', function(e) {
    if (e.target.tagName !== 'TD') return;
    const color = document.querySelector('.color-picker').value;
    e.target.style.backgroundColor = color;
  });
});

</script>
<script>
function convertTo3DigitRGB(rgbColor) {
  // O parâmetro 'rgbColor' deve estar no formato 'rgb(r, g, b)'.
  // Primeiro, removemos o prefixo 'rgb(' e o sufixo ')', usando slice.
  const rgbValues = rgbColor.slice(4, -1).split(', ');

  // Convertemos cada valor para um número inteiro decimal e então formatamos em 1 dígito em hexadecimal.
  const r = parseInt(rgbValues[0]).toString(16).padStart(2, '0').slice(0, 1);
  const g = parseInt(rgbValues[1]).toString(16).padStart(2, '0').slice(0, 1);
  const b = parseInt(rgbValues[2]).toString(16).padStart(2, '0').slice(0, 1);

  // Concatenamos os valores em uma string no formato '0xrgb'.
  const hexColor = `0x${r}${g}${b}`;

  return hexColor;
}

function captureCellColors() {
  const cellArray = []; // Array para armazenar as cores das células

  // Seleciona todas as células da grade usando querySelectorAll
  const cells = document.querySelectorAll('.pixel-canvas td');

  // Itera sobre cada célula para obter sua cor de fundo e adicioná-la à array
  cells.forEach(cell => {
    const color = cell.style.backgroundColor;
    cellArray.push(convertTo3DigitRGB(color)); // Usamos a função de conversão aqui
  });

  return cellArray;
}

function addLineBreaks(cellArray, cellsPerRow) {
  let result = '';

  // Percorremos a array de células e adicionamos quebras de linha conforme necessário.
  cellArray.forEach((cell, index) => {
    result += cell;

    // Adicionamos quebra de linha após cada linha de acordo com a quantidade de células por linha (cellsPerRow).
    const isLastCellInRow = (index + 1) % cellsPerRow === 0;
    const isLastCell = index === cellArray.length - 1;
    if (isLastCellInRow && !isLastCell) {
      result += ',\n';
    } else if (!isLastCell) {
      result += ', ';
    }
  });

  return result;
}


function updateColorArray() {
  enviarAjax();
  const colorsArray = captureCellColors(); // Captura as cores das células em formato de 3 dígitos
  const inputWidth = document.getElementById('input-width').value; // Obtém o valor do input com o id="input-width"
  const colorsWithLineBreaks = addLineBreaks(colorsArray, inputWidth); // Adiciona quebras de linha na string de cores
  const textarea = document.getElementById('color-array'); // Seleciona a <textarea> pelo id
  textarea.value = colorsWithLineBreaks; // Define o conteúdo da <textarea> com a string de cores com quebras de linha
}

function loadColors() {
  const textarea = document.getElementById('color-array'); // Seleciona a <textarea> pelo id
  const colorsString = textarea.value; // Obtém a string de cores do <textarea>
  const colorsArray = colorsString.split(',').map(color => color.trim()); // Divide a string em um array de cores
  const cells = document.querySelectorAll('.pixel-canvas td'); // Seleciona todas as células da grade

  // Itera sobre cada célula e atribui a cor do array (considerando que o array está no formato "#000")
  cells.forEach((cell, index) => {
    cell.style.backgroundColor = colorsArray[index].replace('0x', '#');
  });
}


//paleta de cores
// Seleciona todos os elementos com a classe "color"
const colors = document.querySelectorAll('.color');

// Adiciona um evento de clique a cada cor da paleta
colors.forEach(color => {
  color.addEventListener('click', function() {
    // Obtém a cor de fundo da cor clicada
    const selectedColor = color.style.backgroundColor;
    
    // Define a cor selecionada como a cor do pincel
    document.querySelector('.color-picker').value = rgbToHex(selectedColor);
  });
});

// Função para converter RGB para hexadecimal
function rgbToHex(rgb) {
  // Extrai os valores de R, G e B da string RGB
  const [r, g, b] = rgb.match(/\d+/g);
  
  // Converte os valores de R, G e B para hexadecimal e os concatena
  const hexColor = "#" + ((1 << 24) | (r << 16) | (g << 8) | b).toString(16).slice(1);
  
  return hexColor;
}

document.querySelectorAll('.color').forEach(colorDiv => colorDiv.addEventListener('click', selectColor));



  function selectColor(element) {
    // Obtém a cor de fundo da cor selecionada na paleta
    color = element.style.backgroundColor;
    
    // Define a cor selecionada como a cor do pincel (variável color)
    const colorPicker = document.querySelector('.color-picker');
    colorPicker.value = rgbToHex(color);
  }





//-------------





</script>
<style>
#divisoriasButton.active {
                   background-color: #1519;
                 border: 2px;
                 top: -1px;
                 position: relative;                    
                  box-shadow: 1px 1px 5px #090;
                 animation-duration: 1s;ativo */
}
/* Styles buttons, including a change of displayed mouse cursor to inform user he/she is hovering over something clickable */
.button{
  box-shadow: inset 0px 1px 0 0 #fff;
  background: linear-gradient(#f9f9f9 5%, #e9e9e9 100%);
  /* creates rounded edges (more pixels -> more rounded) */
  border-radius: 7px;
  border: 1px solid #dcdcdc;
  font-weight: bold;
  /* Padding is space between element's content and border. First value sets top and bottom padding; second value sets right and left */
  padding: 6px 24px;
  text-decoration: none;
  /* text-shadow: 0 1px #fff; */
  cursor: pointer;
  /* Places button on its own line */
  margin-top: 1em;
}
         .button {
                
                 border-radius: 7px;
                 margin-top: 4px;
                 margin-bottom: 4px;
                 background-color: #4CAF50dd;
                 border: 2px;
           box-shadow: 1px 1px 1px #55550011; 
           z-index: 10;
            top: -1px;
                 position: relative;

                 
                 transition: box-shadow 0.3s;
               }
               
               .button:hover {
                 background-color: #4CAF50;
                 border: 2px;
                 z-index: 100;
                 top: -3px;
                 position: relative;
                    
                 box-shadow: 1px 1px 15px #0f0a; 
                 transition: box-shadow 0.3s;
               }
               .button:active {
                 background-color: #1519;
                 border: 2px;
                 top: -1px;
                 position: relative;
                    
                  box-shadow: 1px 1px 10px #090;
                 animation-duration: 1s;
               }
               /* Estilizando o botão desabilitado */
.button:disabled {
  background-color: #ccc;
  cursor: not-allowed;
  position: relative;
}

.button:disabled::before {
  content: ""; /* Ícone de proibido */
  color: red;    
  font-size: 20px;
}


body{
  background: linear-gradient(#e9e9e9 5%, #ccc 100%);
}

  #color-palette {
  display: flex;
}

.color{
   display: inline-block;
  border: 1px solid #dcdcdc;
  -webkit-appearance: none;
  padding: 0;
  
  border-radius: 3px;
  width: 20px;
  height: 20px;
  cursor: pointer;
  margin: 2px;
  margin-bottom: 1.5px;
}

tr {
  height: 5px;
}

td {
  min-width: 5px;
  box-sizing: border-box;
}

textarea {
            font-family: "Courier New", monospace;
            font-size: 0.7em;
            width: 85vw;
            height: 150px;
            margin-bottom: 50px;
            text-align: center;
        }

</style>



<br>
<div>

    <button class="button" onclick="convertAndCopyURL()" id="copiaurl">copy URL</button>
    <button class="button" onclick="convertAndCopyMiniURL()" id="copiaminiurl">Copiar mini URL💾</button>
    <a href='./galeria/' target="_blank"> <button class="button"onclick="window.location.href='./galeria/' id='copiaminiurl'"> Galeria🖼️</button></a>


    <button class="button" onclick="baixarPNG()" id="Salvarpng">Baixar PNG</button> 

<button class="button" onclick="loadColors();processarConteudoTextarea();updateBoxSize(getAndPrintValueID('input-scale'));">load</button>

<button class="button" onclick="invertMatrix2();">flip</button>
<button class="button" onclick="copiar();">copy</button>
<button class="button" onclick="invertMatrix();">copy inverted</button>
<!--<button class="button" onclick="">coming soon</button> -->
<!-- Botão personalizado para enviar PNG -->
<label for="imageInput" class="button">Carregar PNG</label>

<input type="file" id="imageInput" accept="image/png">
<input type="file" id="imageInput" class="button">


</div>
<!-- Adicione uma caixa de texto editável para exibir a array -->
<textarea class="array-text" id="color-array" rows="4" cols="50"></textarea>

    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <style>

    </style>


    <script>// Array para armazenar as cores geradas
let coresGeradas = [];

// Função para verificar se a cor já existe na paleta
function corJaExiste(cor) {
    return coresGeradas.includes(cor);
}

// Função para adicionar uma cor à paleta
function adicionarCor(cor) {
    if (!corJaExiste(cor)) {
        const paleta = document.getElementById("paleta");
        const novaDiv = document.createElement("div");
        novaDiv.className = "color";
        novaDiv.style.backgroundColor = cor;
        novaDiv.setAttribute("onclick", `console.log('clicou na cor: ${cor}'); selectColor(this)`);
        paleta.appendChild(novaDiv);
        coresGeradas.push(cor);
    }
}

// Função para converter uma cor no formato "0x..." para o formato "#xxx" ou "#xxxxxx"
function converterCorHexadecimal(cor) {
    let corConvertida = cor.trim().toUpperCase();
    if (corConvertida.startsWith("0X")) {
        corConvertida = "#" + corConvertida.slice(2);
    }
    if (corConvertida.startsWith("#")) {
        const hexPattern = /^#([0-9A-F]{3}){1,2}$/;
        if (!hexPattern.test(corConvertida)) {
            console.error(`Cor inválida: ${cor}`);
            return null;
        }
    } else {
        console.error(`Cor inválida: ${cor}`);
        return null;
    }
    return corConvertida;
}

// Função para processar o conteúdo da textarea
function processarConteudoTextarea() {
    const textareaCores = document.getElementById("color-array");    
    document.getElementById("color-array").value = document.getElementById("color-array").value.replace(/NNN/g, "000");
    const coresInseridas = textareaCores.value.split(/\s*,\s*|\s+/);
    coresInseridas.forEach(cor => {
        const corConvertida = converterCorHexadecimal(cor);
        if (corConvertida) {
            adicionarCor(corConvertida);
        }
    });
}

// Evento de clique no botão "Adicionar cor"
//document.getElementById("btnAdicionarCor").addEventListener("click", processarConteudoTextarea);
//document.getElementById("input-scale").addEventListener("change", processarConteudoTextarea);





window.addEventListener("load", () => {
  const boxes = document.querySelectorAll(".td");
  const inputScale = document.querySelector(".input-scale");
  // Evento que atualiza o tamanho das divs quando o valor do input é alterado
  inputScale.addEventListener("change", () => {
    const scaleValue = inputScale.value;
    boxes.forEach(box => {
      box.style.width = scaleValue + "px";
      box.style.height = scaleValue + "px";
    });
  });
});

</script>

  <script>
        // Função para obter o valor de um parâmetro da URL por nome
        function getParametroDaURL(nome) {
            const urlSearchParams = new URLSearchParams(window.location.search);
            return urlSearchParams.get(nome);
        }

        document.getElementById('copiar-url').addEventListener('click', function() {
            const conteudoTextArea = document.getElementById('color-array').value;
            const conteudoCodificado = encodeURIComponent(conteudoTextArea);
            const urlComParametro = window.location.origin + '/PixelEditor/pixel2/?w='+document.getElementById('input-width').value+'&h='+document.getElementById('input-height').value+'&s='+document.getElementById('input-scale').value+''+'&array=' + conteudoCodificado;

            navigator.clipboard.writeText(urlComParametro).then(function() {
                alert('Array copied!');
            }).catch(function(error) {
                console.error('Falha ao copiar a URL: ', error);
            });
        });

        // Verificar se há um parâmetro "array" na URL ao carregar a página
        document.addEventListener('DOMContentLoaded', function() {
            const parametroArray = getParametroDaURL('array');
            if (parametroArray !== null) {
                // Decodificar o valor do parâmetro (caso esteja codificado)
                const arrayDecodificada = decodeURIComponent(parametroArray);
                document.getElementById('color-array').value = arrayDecodificada;
            }
        });
        document.addEventListener('DOMContentLoaded', function() {
            const parametroArray = getParametroDaURL('array');
            if (parametroArray !== null) {
                // Decodificar o valor do parâmetro (caso esteja codificado)
                const arrayDecodificada = decodeURIComponent(parametroArray);
                document.getElementById('color-array').value = arrayDecodificada;
            }
        });
    </script>


        <style>
        /* Estilos para o popup */
        .popup {
            display: none;
            position: fixed;
            top: 50%;
            left: 50%;
            transform: translate(-50%, -50%);
            padding: 10px;
            background-color: #f1f1f1;
            border: 1px solid #ccc;
            border-radius: 5px;
            z-index: 9999;
        }

        .popup.show {
            display: block;
        }
    </style>
<div class="popup" id="popup">Array copied!</div>

    <script>
        document.getElementById('color-array').addEventListener('click', function() {
            // Seleciona todo o conteúdo da textarea
            this.select();

            // Copia o conteúdo selecionado para a área de transferência
            //document.execCommand('copy');

            // Exibe o popup com a mensagem "Array copiada"
            //const popup = document.getElementById('popup');
            //popup.classList.add('show');

            // Esconde o popup após 2 segundos
            //setTimeout(function() {
            //    popup.classList.remove('show');
            //}, 2000);
        });
    </script>



<!DOCTYPE html>
<html>
<head>
    <title>Conversão de Parâmetro em Array</title>
</head>
<body>
  <!-- Formulário oculto para enviar os parâmetros via POST -->
<form id="downloadForm" method="post" action="https://rerre.net/PixelEditor/saveimg.php?action=download" target="_blank">
    <input type="hidden" name="h" id="input-height" value="">
    <input type="hidden" name="w" id="input-width" value="">
    <input type="hidden" name="s" id="input-scale" value="">
    <input type="hidden" name="array" id="input-array" value="">
    <input type="hidden" name="array" id="action" value="download">
    <input type="hidden" name="res" value="original">
</form>

    <script>
      function salvarpng() {
        const downloadForm = document.getElementById('downloadForm');
        downloadForm.elements['h'].value = getAndPrintValueID('input-height');
        downloadForm.elements['w'].value = getAndPrintValueID('input-width');
        downloadForm.elements['s'].value = getAndPrintValueID('input-scale');
        downloadForm.elements['array'].value = convertArrayToParam().replace(/,/g, '_');

        // Submeter o formulário para baixar o PNG
        downloadForm.submit();
    }


        // Função para converter o array para parâmetro da URL
        function convertArrayToParam() {
            const arrayTextarea = document.getElementById('color-array');
            const arrayValue = arrayTextarea.value.trim();

            if (arrayValue) {
                const arrayItems = arrayValue.split(',');
                const resultArray = arrayItems.map(item => item.trim().replace('0x', ''));

                return resultArray.join('_');
            } else {
                alert('Insira um array válido na textarea.');
                return '';
            }
        }

        // Função para copiar a URL convertida para a área de transferência
        function copyConvertedURL(convertedValue,mini,dl) {

            if (convertedValue) {
                const convertedURL = window.location.href.split('?')[0] + '?h='+getAndPrintValueID('input-height')+'&w='+getAndPrintValueID('input-width')+'&s='+getAndPrintValueID('input-scale')+'&array=' + convertedValue.replace(/,/g, '_');


              if(mini == true){
             const convertedURL = window.location.href.split('?')[0] + '?h='+getAndPrintValueID('input-height')+'&w='+getAndPrintValueID('input-width')+'&array=' + convertedValue.replace(/,/g, '_');
                shortenURL(convertedURL);
                //return;
              }
              else{
            
                // Copiar a URL para a área de transferência
                const dummyElement = document.createElement('textarea');
                dummyElement.value = convertedURL;
                document.body.appendChild(dummyElement);
                dummyElement.select();
                document.execCommand('copy');
                document.body.removeChild(dummyElement);
               // alert('URL copied!');

              if(dl == true){
                var url = convertedURL+"&download=1";
                window.open(url, '_blank');
              } else{
                alert('URL copied!');

              }




            }}
        }

        // Função para converter o array e copiar a URL convertida
        function convertAndCopyURL() {
            const convertedValue = convertArrayToParam();
            copyConvertedURL(convertedValue,false,false);
        }


        function convertAndCopyMiniURL() {
            const convertedValue = convertArrayToParam();
            copyConvertedURL(convertedValue,true,false);
        }

        function enviarAjax() {
            const convertedValue = "./aovivo?array=="+convertArrayToParam();
            console.error(convertedValue);
            sendTextAjax(convertedValue);

        }


        function baixarPNG() {
            const convertedValue = convertArrayToParam();
            copyConvertedURL(convertedValue,false,true);
        }




        var encurtado = "";

        function shortenURL(originalUrl) {
            var xhr = new XMLHttpRequest();
            xhr.onreadystatechange = function () {
                if (xhr.readyState == 4 && xhr.status == 200) {
                    var shortenedUrl = xhr.responseText.trim();
                    encurtado = "https://rerre.net/PixelEditor/?url="+shortenedUrl;

                    copyToClipboard(encurtado.trim());

                  //  var shortenedUrlContainer = document.getElementById("shortenedUrlContainer");
//
                  //  // Adiciona a URL encurtada completa ao conteúdo HTML da div
                  //  var outputHtml = "<p>URL encurtada completa: <a href='" + encurtado + "'>" + encurtado + "</a></p>";
//
                  //  // Adiciona um botão para copiar a URL encurtada para a área de transferência
                  //  outputHtml += "<button onclick='copyToClipboard(encurtado)'>Copiar para a área de transferência</button>";
//
                  //  // Atualiza o conteúdo da div com o botão de cópia
                  //  shortenedUrlContainer.innerHTML = outputHtml;
                }
            };

            xhr.open("POST", "mini.php", true);
            xhr.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");
            xhr.send("original_url=" + encodeURIComponent(originalUrl));
        }

        // Função para copiar texto para a área de transferência
        function copyToClipboard(text) {
            var textArea = document.createElement("textarea");
            textArea.value = text.trim();
            document.body.appendChild(textArea);

            // Seleciona o conteúdo do textarea
            textArea.select();
            document.execCommand("copy");
            console.log("Texto copiado com sucesso:", text);

            console.log('Texto copiado para a área de transferência: ' + text);

            // Remove o textarea do DOM
            document.body.removeChild(textArea);
        }

document.addEventListener('DOMContentLoaded', function() {
    const urlSearchParams = new URLSearchParams(window.location.search);
    const arrayTextarea = document.getElementById('color-array');
    let paramValue = urlSearchParams.get('array');

    if (!paramValue) {
        // O parâmetro 'array' não foi encontrado na URL via GET, tentativa de obter do PHP (embora não seja recomendado)
        paramValue = '';
    }

    if (paramValue) {
        const arrayItems = paramValue.split('_');
        const resultArray = arrayItems.map(item => '0x' + item);
        arrayTextarea.value = resultArray.join(',');
    } else {
        arrayTextarea.value = ''; // O parâmetro "array" não foi encontrado na URL via GET ou POST.
    }
});

    </script>
</body>
</html>
        <script>
          function getAndPrintValueID(id) {
    // Seleciona o elemento de input pelo ID fornecido como parâmetro
    const inputElement = document.getElementById(id);

    if (inputElement) {
      // Pega o valor atual do campo
      const valorCampo = inputElement.value;

      // Imprime o valor na console
      console.log(valorCampo);

      // Retorna o valor para poder usá-lo posteriormente, se necessário
      return valorCampo;
    } else {
      console.error("Elemento com ID '" + id + "' não encontrado.");
    }
  }

  // Chamando a função para pegar e imprimir o valor do campo com ID "input-scale"
  console.log(getAndPrintValueID('input-scale'));

          // Função a ser executada quando a página terminar de carregar
        function onPageLoad() {
            console.log('A página terminou de carregar.');
            // Coloque aqui os comandos que deseja executar após o carregamento da página.
            loadColors();
            processarConteudoTextarea();
            updateBoxSize(getAndPrintValueID('input-scale'));
            updateColorArray();

        }

        // Adiciona um ouvinte de evento para o evento DOMContentLoaded
        document.addEventListener('DOMContentLoaded', onPageLoad);
        console.clear();
        updateBoxSize(getAndPrintValueID('input-scale'));


    </script>


    
    <div class="popup" id="popup">Copiado!</div>
    <script>
        function invertMatrix() {
            const inputText = document.getElementById('color-array').value.trim();
            const lines = inputText.split('\n');
            let outputText = '';

            for (let i = 0; i < lines.length; i++) {
                const lineValues = lines[i].split(',');
                const reversedLine = lineValues.map(value => value.trim()).reverse();
                let movedCommas = reversedLine.join(',') + ',';
                if (movedCommas.startsWith(',')) {
                    movedCommas = movedCommas.substring(1); // Remove a vírgula no início da linha, se houver
                }
                outputText += movedCommas + '\n'; // Modificar <br> para \n para manter o formato da textarea
            }

            // Atualizando o valor da textarea com o texto invertido
            const inputTextArea = document.getElementById('color-array');
            inputTextArea.value = outputText;


            // Selecionando o conteúdo da textarea
            //inputTextArea.select();
            //inputTextArea.setSelectionRange(0, 99999); // Para dispositivos móveis

            // Copiando o texto selecionado para a área de transferência
            //document.execCommand('copy');

            // Exibindo o popup informativo
            const popup = document.getElementById('popup');
            popup.style.display = 'block';
            setTimeout(function() {
                popup.style.display = 'none';
                //loadColors();processarConteudoTextarea();
                inputTextArea.select();
                document.execCommand('copy');
                updateColorArray();
            }, 500); // Esconde o popup após 2 segundos
        }

        function invertMatrix2() {
            const inputText = document.getElementById('color-array').value.trim();
            const lines = inputText.split('\n');
            let outputText = '';

            for (let i = 0; i < lines.length; i++) {
                const lineValues = lines[i].split(',');
                const reversedLine = lineValues.map(value => value.trim()).reverse();
                let movedCommas = reversedLine.join(',') + ',';
                if (movedCommas.startsWith(',')) {
                    movedCommas = movedCommas.substring(1); // Remove a vírgula no início da linha, se houver
                }
                outputText += movedCommas + '\n'; // Modificar <br> para \n para manter o formato da textarea
            }

            // Atualizando o valor da textarea com o texto invertido
            const inputTextArea = document.getElementById('color-array');
            inputTextArea.value = outputText;


            // Selecionando o conteúdo da textarea
            //inputTextArea.select();
            //inputTextArea.setSelectionRange(0, 99999); // Para dispositivos móveis

            // Copiando o texto selecionado para a área de transferência
            //document.execCommand('copy');

            // Exibindo o popup informativo
            //const popup = document.getElementById('popup');
            //popup.style.display = 'block';
            setTimeout(function() {
                //popup.style.display = 'none';
                loadColors();processarConteudoTextarea();
                //inputTextArea.select();
                //document.execCommand('copy');
                updateColorArray();
            }, 1); // Esconde o popup após  segundos
        }

                function copiar() {
            const inputText = document.getElementById('color-array').value.trim();
            const lines = inputText.split('\n');
            let outputText = '';

            for (let i = 0; i < lines.length; i++) {
                const lineValues = lines[i].split(',');
                const reversedLine = lineValues.map(value => value.trim()).reverse();
                let movedCommas = reversedLine.join(',') + ',';
                if (movedCommas.startsWith(',')) {
                    movedCommas = movedCommas.substring(1); // Remove a vírgula no início da linha, se houver
                }
                outputText += movedCommas + '\n'; // Modificar <br> para \n para manter o formato da textarea
            }

            // Atualizando o valor da textarea com o texto invertido
            const inputTextArea = document.getElementById('color-array');
            inputTextArea.value = outputText;


            // Selecionando o conteúdo da textarea
            inputTextArea.select();
            inputTextArea.setSelectionRange(0, 99999); // Para dispositivos móveis

            // Copiando o texto selecionado para a área de transferência
            //document.execCommand('copy');

            // Exibindo o popup informativo
            const popup = document.getElementById('popup');            
            document.execCommand('copy');
            popup.style.display = 'block';
            setTimeout(function() {
                popup.style.display = 'none';
                //loadColors();processarConteudoTextarea();
                                //updateColorArray();
            }, 2000); // Esconde o popup após  segundos
        }

        // Removendo a função copyToClipboard, pois não é mais necessária
    </script>
    <script type="text/javascript">//funçao pra bloquar o botao de copiar url
      function checkMultiplicationAndChangeButtonContent() {
      const inputWidth = document.getElementById('input-width');
      const inputHeight = document.getElementById('input-height');
      const widthValue = Number(inputWidth.value);
      const heightValue = Number(inputHeight.value);
      const multiplicationResult = widthValue * heightValue;
      const btn2 = document.getElementById('copiaurl');

      if (isNaN(widthValue) || isNaN(heightValue) || widthValue <= 0 || heightValue <= 0) {
        btn2.disabled = true;
      } else {
        btn2.disabled = false;
        if (multiplicationResult > 2025) {
          btn2.textContent = '-----';//⚠️Longo Demais⚠️
        } else {
          btn2.textContent = 'copy URL';
        }
      }
    }

    document.getElementById('input-width').addEventListener('input', checkMultiplicationAndChangeButtonContent);
    document.getElementById('input-height').addEventListener('input', checkMultiplicationAndChangeButtonContent);
    </script>

    <script type="text/javascript">
      // Função para detectar qual quadradinho da malha foi clicado, movido ou solto
// Função para detectar qual quadradinho da malha foi clicado, movido ou solto
// Variável para armazenar a última célula detectada
let lastDetectedCell = null;

// Função para detectar qual quadradinho da malha foi clicado, movido ou solto
// Variável para armazenar o número exclusivo de cada quadradinho
let cellNumber = 0;

// Função para detectar qual quadradinho da malha foi clicado, movido ou solto
// Função para detectar qual quadradinho da malha foi clicado, movido ou solto
function detectClickedSquare(event) {
  // Verifica se o elemento clicado é uma célula (TD) da malha
  if (event.target.tagName === 'TD') {
    // Obtém a linha (TR) pai da célula clicada
    const row = event.target.parentElement;
    // Obtém o índice da coluna da célula clicada dentro da linha
    const colIndex = Array.from(row.children).indexOf(event.target);
    // Obtém o índice da linha da célula clicada dentro da malha
    const rowIndex = Array.from(pixelCanvas.children).indexOf(row);

    // Obtém a cor atual do quadradinho clicado
    const color = event.target.style.backgroundColor;

    // Obtém o número de colunas da malha
    const numCols = row.children.length;

    // Atribui um número exclusivo ao quadradinho
    const cellUniqueId = colIndex + rowIndex * numCols;

    // Exibe a posição, o número exclusivo e a cor do quadradinho clicado/movido/solto no console
    console.log(`Quadradinho: Linha ${rowIndex + 1}, Coluna ${colIndex + 1}, Número exclusivo: ${cellUniqueId}, Cor: ${color}`);
  }
}





// Variável para rastrear se o botão do mouse está pressionado
let mouseIsDown = false;

// Evento de clique na malha para detectar qual quadradinho foi clicado
pixelCanvas.addEventListener('click', detectClickedSquare);

// Evento de pressionar o botão do mouse na malha
pixelCanvas.addEventListener('mousedown', function(event) {
  mouseIsDown = true;
  // Chama a função para detectar o quadradinho quando o botão é pressionado
  detectClickedSquare(event);
});

// Evento de soltar o botão do mouse na malha
pixelCanvas.addEventListener('mouseup', function() {
  mouseIsDown = false;
});

// Evento de movimentar o mouse sobre a malha
pixelCanvas.addEventListener('mousemove', function(event) {
  // Verifica se o botão do mouse está pressionado (mousedown)
  if (mouseIsDown) {
    // Chama a função para detectar o quadradinho enquanto o mouse é movido com o botão pressionado
    detectClickedSquare(event);
  }
});

// Array para armazenar o estado da malha em cada modificação
let gridStates = [];

// Função para salvar o estado atual da malha na memória
function saveGridState() {
  // Cria uma cópia do estado atual da malha usando o método slice()
  const gridStateCopy = Array.from(pixelCanvas.querySelectorAll('td')).map(td => td.style.backgroundColor);
  // Adiciona a cópia ao array de estados
  gridStates.push(gridStateCopy);
}

// Função para desfazer a última modificação na malha (Ctrl+Z)
function undoLastModification() {
  if (gridStates.length > 1) { // Verifica se há mais de um estado na memória
    gridStates.pop(); // Remove o estado atual da memória
    const previousState = gridStates[gridStates.length - 1]; // Obtém o estado anterior
    const gridCells = pixelCanvas.querySelectorAll('td'); // Seleciona todas as células da malha

    // Restaura as cores dos quadradinhos para o estado anterior
    for (let i = 0; i < gridCells.length; i++) {
      gridCells[i].style.backgroundColor = previousState[i];
    }
  }
}

// Evento de clique na malha para detectar qual quadradinho foi clicado e salvar o estado
pixelCanvas.addEventListener('click', function(event) {
  detectClickedSquare(event); // Exibe a posição e a cor do quadradinho clicado no console
  saveGridState(); // Salva o estado atual da malha na memória
});

// Evento de pressionar a tecla (usado para o "Ctrl+Z")
document.addEventListener('keydown', function(event) {
  if (event.ctrlKey && event.key === 'z') {
    event.preventDefault(); // Impede o comportamento padrão do "Ctrl+Z" no navegador
    undoLastModification(); // Desfaz a última modificação na malha
  }
});

    </script>
    <script type="text/javascript">
      // JavaScript para mostrar e ocultar as linhas ao marcar/desmarcar a checkbox
// JavaScript para mostrar e ocultar as linhas ao clicar no botão
function showDivisorias() {
  const button = document.getElementById('divisoriasButton');
  const pixelCanvas = document.querySelector('.pixel-canvas');
  let horizontalDiv = document.querySelector('.div-horizontal');
  let verticalDiv = document.querySelector('.div-vertical');

  if (!button.classList.contains('active')) {
    // Cria as linhas apenas se elas ainda não existirem
    if (!horizontalDiv) {
      horizontalDiv = document.createElement('div');
      horizontalDiv.classList.add('div-horizontal');
      pixelCanvas.appendChild(horizontalDiv);
    }
    if (!verticalDiv) {
      verticalDiv = document.createElement('div');
      verticalDiv.classList.add('div-vertical');
      pixelCanvas.appendChild(verticalDiv);
    }

    button.classList.add('active');
  } else {
    // Remove as linhas se o botão estiver ativo
    if (horizontalDiv) {
      pixelCanvas.removeChild(horizontalDiv);
    }
    if (verticalDiv) {
      pixelCanvas.removeChild(verticalDiv);
    }

    button.classList.remove('active');
  }
}

// Adiciona um ouvinte de evento para chamar a função quando o botão é clicado
document.getElementById('divisoriasButton').addEventListener('click', showDivisorias);



// Adiciona um ouvinte de evento para chamar a função quando a checkbox é clicada
document.getElementById('divisorias').addEventListener('change', showDivisorias);
    </script>

    <style type="text/css">

      /* CSS para posicionar a tabela e a div */
.pixel-canvas {
  position: relative;
  /* Adicione outras propriedades de estilo conforme necessário */
}

.div-horizontal {
  position: absolute;
  top: 50%;
  left: 0;
  width: 100%;
  height: 3px;
  background-color: #080;
}

.div-vertical {
  position: absolute;
  top: 0;
  left: 50%;
  width: 3px;
  height: 100%;
  background-color: #080;
}



.tabelapixel {  
  width: 80vw;
  display: flex;
  justify-content: center;
  align-items: center;
}
.row { 
  width: 100vw;
  display: flex;
  justify-content: center;
}


.inputs {
  
  text-align: left;
  margin-left: 8vw;
  margin-top: 5.35em;
  border-radius: 10px;
  border: 2px solid #dcdcdc;
  padding-left: 1.3em;
  padding-top: 0.5em;
  padding-bottom: 1.5em;
  width: 200px;
  background-color : white;
}

input{
   margin:3px;
}

    
    </style>


  <style>
    /* Estilos personalizados */
    #imageInput {
      display: none; /* Esconde o input de arquivo */
    }

  </style>
</head>
<body>

  <canvas id="canvas" style="display: none;"></canvas>

  <script>
// Função para converter uma imagem em uma matriz de cores representadas em hexadecimal
function convertImageToColorArray(imageUrl) {
  const img = new Image();

  // Evento que é acionado quando a imagem é carregada
  img.onload = function () {
    const canvas = document.getElementById('canvas');
    const ctx = canvas.getContext('2d');

    // Configura a largura e altura do canvas para a largura e altura da imagem
    canvas.width = img.width;
    canvas.height = img.height;

    // Desenha a imagem no canvas
    ctx.drawImage(img, 0, 0);

    // Obtém os dados da imagem (pixels) do canvas
    const imageData = ctx.getImageData(0, 0, canvas.width, canvas.height);
    const pixels = imageData.data;

    // Array que irá conter as cores da imagem em formato hexadecimal
    const colorArray = [];

    // Loop através de todos os pixels (cada pixel é composto por 4 valores: R, G, B e A)
    for (let i = 0; i < pixels.length; i += 4) {
      const r = pixels[i];     // Valor do componente vermelho (0 a 255)
      const g = pixels[i + 1]; // Valor do componente verde (0 a 255)
      const b = pixels[i + 2]; // Valor do componente azul (0 a 255)
      const a = pixels[i + 3]; // Valor do componente alfa (transparência) (0 a 255)

      // Converte cada componente para hexadecimal
      const hexR = r.toString(16).padStart(2, '0').slice(0, 1); // Componente vermelho em hexadecimal
      const hexG = g.toString(16).padStart(2, '0').slice(0, 1); // Componente verde em hexadecimal
      const hexB = b.toString(16).padStart(2, '0').slice(0, 1); // Componente azul em hexadecimal

      // Combina os componentes para obter o valor hexadecimal de 3 dígitos (exemplo: '#ABC')
      const hexColor = `0x${hexR}${hexG}${hexB}`;

      // Adiciona o valor hexadecimal da cor ao array de cores
      colorArray.push(hexColor);
    }

    // Neste ponto, `colorArray` conterá o array de cores da imagem em formato hexadecimal

    // Exibe o array de cores no elemento de textarea
    const colorArrayTextArea = document.getElementById('color-array');
    colorArrayTextArea.value = colorArray.join(', ');

    // Chama a função invertMatrix2() duas vezes (não há a explicação dessa função nos comentários)
    invertMatrix2();
    invertMatrix2();
  };

  // Define a URL da imagem a ser carregada
  img.src = imageUrl;
}

// Evento acionado quando o input de arquivo muda (quando um arquivo de imagem é selecionado)
document.getElementById('imageInput').addEventListener('change', function (event) {
  const file = event.target.files[0];
  const reader = new FileReader();

  // Evento acionado quando a leitura do arquivo é concluída
  reader.onload = function (event) {
    const imageUrl = event.target.result;
    // Chama a função para converter a imagem em uma matriz de cores
    convertImageToColorArray(imageUrl);
  };

  // Inicia a leitura do arquivo como uma URL de dados (DataURL)
  reader.readAsDataURL(file);
});

  </script>


)rawliteral";

//ESP8266WebServer server(80);

void desenhar2(String mensagem){//criei essa função igual a outra pois eu não consegui chamar a desenher(), 
controle=9;
//matrix->clear();
//limpa();
    uint16_t desenhoo[1][64];//resolver depois
    mensagem.replace("_","0x");
    mensagem.replace("=","0x");
    String a = "";
    char *end;
    //"0x6E3"
    Serial.println(mensagem);
    //chat[0].c_str(ircMessage.text);
    
    int x=0;
    for (int i = 0 ;i<mensagem.length() ; i=i+5){
      a = mensagem.substring(i+2,i+5);
      desenhoo[0][x]=(uint16_t)strtoul(a.c_str(), NULL, 16);
      x++;
      //Serial.println((uint16_t)strtoul(a.c_str(), NULL, 16));
    }
    fixdrawRGBBitmap(0, 0, desenhoo[0], 8, 8);
}

void handleRoot() {  
  Serial.println(server.arg("array"));
  desenhar2(server.arg("array"));

  server.send(200, "text/html", index_html);
}

void handleaovivo() {  
  Serial.println(server.arg("array"));
  desenhar2(server.arg("array"));
  server.send(200, "text/html", "ok"); 
}

void setupWebServer() {
  server.on("/", handleRoot);
  server.on("/aovivo", handleaovivo);
  server.begin();
  Serial.println("HTTP server started");
}

void loopserver(){
  server.handleClient();
}

#endif // WEBSERVER_H
