// HTML web page
// As imagens dos botões foram codificadas em base 64
// background: url('data:image/png;base64, IMAGEM EM BASE 64') no-repeat top left;

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1, maximum-scale=1, minimum-scale=1, user-scalable=no, minimal-ui">
    <head>
        <style>
            .button {
            background-color: #4CAF50; /* Green */
            border: none;
            color: white;
            text-align: center;
            text-decoration: none;
            display: inline-block;
            margin: 4px 2px;
            transition-duration: 0.6s;
            cursor: pointer;
            }

            .button1 {
            background-color: white; 
            color: rgb(114, 99, 99); 
            border: 2px solid #4CAF50;
            }

            .button1:hover {
            background-color: #4CAF50;
            color: white;
            }
            
            .btnup {
                position: absolute;
                left: 136px;
                transition-duration: 0.6s;
                
                border: none;
                background: url('data:image/png;base64,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') no-repeat top left;
                padding: 2px 8px;
                width: 128px ;
                height: 128px;
                background-color: white;
            }
            
     

            .btnup:active {
                background-color: #4CAF50;

                background: url('data:image/png;base64,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') no-repeat top left;
                background-color: #4CAF50;
            }

            .btndown {
                position: absolute;
                left:136px;
                top:264px;
                
                transition-duration: 0.6s;
                
                border: none;
                background: url('data:image/png;base64,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') no-repeat top left;
                padding: 2px 8px;
                width: 128px ;
                height: 128px;
                background-color: white;
            }
    

            .btndown:active {
                background-color: #4CAF50;

                background: url('data:image/png;base64,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') no-repeat top left;
                background-color: #4CAF50;
            }

            .btnleft {
                position: absolute;

                top: 136px;
                transition-duration: 0.6s;
                
                border: none;
                background: url('data:image/png;base64,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') no-repeat top left;
                padding: 2px 8px;
                width: 128px ;
                height: 128px;
                background-color: white;
            }
        

            .btnleft:active {
                background-color: #4CAF50;

                background: url('data:image/png;base64,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') no-repeat top left;
                background-color: #4CAF50;
            }

            .btnright {
                position: absolute;
                left:264px;
                top:136px;
                transition-duration: 0.6s;
                
                border: none;
                background: url('data:image/png;base64,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') no-repeat top left;
                padding: 2px 8px;
                width: 128px ;
                height: 128px;
                background-color: white;
            }
            

            .btnright:active {
                background-color: #4CAF50;

                background: url('data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAIAAAACACAYAAADDPmHLAAAAAXNSR0IArs4c6QAAAARnQU1BAACxjwv8YQUAAAAJcEhZcwAADsMAAA7DAcdvqGQAAAeiSURBVHhe7Z1ZrJ1TFMev4cFYBC9EzRI8aBGhCUIaJIjhQShBkJCIVyUxqxjiFQ9FTW9qFgnhRVJezKSIIKqIoWlpDSHC/5c6D3XXHb5z97C+/a1f8kuk7j17Wvfec/a39toTQRAEQRAEQRAEQRAEQdBntpH7yOPlxfIGeZ9cKV+Vq+Rb8l35nnxbviFfk0/J++VN8hJ5gpwvt5WBU+ZJFvta+aT8WG6S/yTyV/mJJICWyhMlbQYV2VdeJlnwb6S1cDmlTQKCPtCXoAA7ywvkc3KjtBamhvy2eV4ukfGbIQMHyTvlV9JaAE+ukXfJg2UwR46QD0v+BluT7dnf5Aq5QAYd4Sd+ufxTWpPbJxnDQzJ+I8yCneRt8mdpTWaf/UXeLnkfExicLfn4Zk1eS/Jx8hwZ/Mfukr+V1mS17CNyDzloTpGfS2uChiBjP1UOkhulNSlDlG3nwcCbIHbQrIkYsjyDaH4TaX/JgxdrAsKJiXckc9QkbIj0YSevtuwkLpRNcaz8UVoDDif7k1wkm4DFXy+tgYZTu0H2PgjYyyearQGGM7tO9vbPAc/Iv5bWwGq7Wl4lj5ZM8EWSLCHra2u7Vu4newV7+qRaWQOq7cvS+ri1teShjfU9tX1f9uoZwtPSGkhtWfzt5HQ8KK3vre2zshd43eGbzeKP8BoEN0vXnCytjte2y+KP8BoEi6VLdpUeN3rGWfwRHoOAN9a7SXc8Jq0O13Quiz/CYxA8IV1xurQ6WtNX5FwXf4THIDhTumB76e2Z/kdyB5kSb0HwpUw9xrG4VVodrClnB3LgLQiWyaqw25fyCFYKycI9UObCUxCQel718TE5+1bHavqH5DBnTjwFwaOyCodKr3n7J8nceAmCv+RhsjhktVod8iBnB0vgJQj4CF4Unk7xq9bqjBdvkSXw8ACJtThAFoPDj1ZHvFlq79xDENwji7Cj/FZanfDoUILgO8lj+OycL60OeHYoQZBr/2MLXpRW494dQhC8JLOyl+zjWf2RrQcBG0N7y2xQD8dquE+2HgRXyGx4TfXqastBkC11jHeY30ur0T7aahD8ILMkkFKHz2qwz7YaBNQxTA4FEq3G+m6LQXC9TA7Hl63GWrC1IHhGJoXau59Kq7FWZCt1K5mbEkHAWrFmyaDwMp8xrcbGlceYdJQTRJx8qemH8jN5nixB7iD4XbJmyUj9BpBEEp5he6rAzdEwnnOUIncQUOU8GZRctxoZx2JPrXpAziC4VCaDevtWI10lWzfYklxBkLTwFBcoWI10lUqZwWRyBMEDMhnU5Lca6erVMrBJHQR8bE8G16xYjXT1GhlMTcog4PqbZHCfjtVIVyMAZuZ1ac1dV9+UyeCCJauRrkYAzMwZ0pq7rlJzMBncrmU10tUIgJlJlXHNBlcyeDGrka5GAMwMx+3cBUCq8q4RADOT6qh90j8B8SawHC7fBPKRwmqkqxEA0+P2Y2CqXIDYCJqa1OcMk24Esa1oNdLV2Aq2yXHINOlWMA8WrEa6Gg+DJpPrhHHSh0Hcom01Mo53y2AzOY+XJ30cTHKB1ci4khBCgQlPCSGkg5VMCMldWyBpQgjpRaQZWQ2NKylh3KHHLuMoNauWH8iSKWG5Fz95StgQkkLvlaSF5Sb34mPypFAg1dhqrAVLVRMpsfiYPC0crpNWY323tcXHLAdDUr8R9GCLi49ZjoZx4LClw6GtLn62w6HQyvuAVhcfs/z9H0HxAavRPtny4mPWAhGUH0l9RKykrS9+9hIxQCEiq3Hvtr74mL1IFFCKzGrcs0NYfCxSJo5SMX0qFDmUxS9WKBJ4omd1wptDWXwseuiWSwq8F4vmJpMSeFj84sWiweMtYSOTpkNNg4fFx+Ll4oECDzzStTpU07/lMTI3XhafNThcVsHjpRHrJZdY5sTL4mO1K2OAY0zeagdvlHvKXHha/OqXRgGZvlbnanqWzIGnxcc7ZHW4vPALaXWwlqR4DeHiyJI5jNOS6khzSlPcGzxiubTaqKmbq2NHPC6tjtY0RRB4XHx3l0cDV5qvkVaHazqXIPC4+G6vj4fF0up0bccJAo+Lj8yxa1IdI0ttlyDwuvililrPGa+pY7MJAq+Ln+0mkByQlMiJG2sgtSUI5sn/w6EQbx/1RjKX2RI9c8Eu4VppDai2q+WV8ii5QF4oV0nra2vLHDKXvYTJXSetgYUzy9wtlL3mOLlBWgMMp5Y5WySbgIHEb4LZy1w1s/gjjpQeN4q8yRwxV01C2hL166yBh5vnpnhqV2n4CNbyDWTjyq2s1sfTZmFXy5qIIdqbHb7UnCa95RKUlLEzB4OG9C3y2qwJalnGnDN1rXecK1uvQ4SMkbEGBux3L5MkdlqT12cZE2Pr3Z5+DQ6R1BL0eO6gq4yBsTCmoCM8S1gh+1iTgD7Td8YQzBF+ejiQ2oedRPpIX+MnPgO7yCXyBblJWgtQQ/pCn+gbfQwKwN06l8uVska9AtqkbfrQ2+f1rcBPHbXxlkq2malBnPLIGq/Fa/LatEFb8ZPuGKqQz5cUt6TUPYmq3H/MAnKVCncg8eBlVCya/+bf+H98DV/L9/C9vAav5amyeRAEQRAEQRAEQRAEQRB0YWLiX3W4PrDQAw2BAAAAAElFTkSuQmCC') no-repeat top left;
                background-color: #4CAF50;
            }
            
            
           
            
            </style>
        <title>
           Aplicação carrinho
        </title>
    </head>
    <body>
        <div style="display:block; width: 388px ; height: 400px">
            <button onmousedown="sendValue('fader1=1');" ontouchstart="sendValue('fader1=1');" onmouseup="sendValue('fader1=0');" ontouchend="sendValue('fader1=0');" class="btnup"></button>
            <script>
                function sendValue(x) {
                  var xhr = new XMLHttpRequest();
                  xhr.open("GET", "/?" + x, true);
                  xhr.send();
                }
                function saiu() {
                  var xhr = new XMLHttpRequest();
                  xhr.open("GET", "/?fader1=0&fader2=0", true);
                  xhr.send();
                               
                }
            </script>
            
            <button onmousedown="sendValue('fader2=1');" ontouchstart="sendValue('fader2=1');" onmouseup="sendValue('fader2=0');" ontouchend="sendValue('fader2=0');" class="btnleft"></button>
            <button onmousedown="sendValue('fader2=-1');" ontouchstart="sendValue('fader2=-1');" onmouseup="sendValue('fader2=0');sendValue('fader2=0');" ontouchend="sendValue('fader2=0');" class="btnright"></button>
            <button onmousedown="sendValue('fader1=-1');" ontouchstart="sendValue('fader1=-1');" onmouseup="sendValue('fader1=0');" ontouchend="sendValue('fader1=0');" class="btndown"></button>
        </div>
        <div style= " padding: 50px 0px 0px 0px;display:block">
            <label for="distx">Referencia em X(cm):</label>
            <input type="number" id="distx" name="distx" min="0" max="20" style="width:60px">
            <button class="button button1" onmousedown="mover(document.getElementById('distx').value);" ontouchstart="mover(document.getElementById('distx').value);" style="width:120px">Enviar Posição</button>
            <script>
              function mover(x) {
                    var xhr = new XMLHttpRequest();
                    xhr.open("GET", "/?" + "fader3=" + parseFloat(x), true);
                    xhr.send();
                  }
            </script>
        </div>
        <div style= "display:block">
            <label for="giro" style="padding:0px 16px 0px 0px" >Referencia em Z(º):</label>
            <input type="number" id="giro" name="giro" min="-180" max="180" style="width:60px">
            <button class="button button1"  onmousedown="girar(document.getElementById('giro').value);" ontouchstart="girar(document.getElementById('giro').value);" style="width:120px">Enviar Ângulo</button>
            <script>
              function girar(x) {
                    var xhr = new XMLHttpRequest();
                    xhr.open("GET", "/?" + "fader4=" + parseFloat(x), true);
                    xhr.send();
                  }
            </script>
        </div>
    </body>
</html>)rawliteral";