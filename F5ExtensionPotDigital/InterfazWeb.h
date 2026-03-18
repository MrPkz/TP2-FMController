#include <arduino.h>

// ---------------- Página HTML con AJAX y pines visibles ----------------

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
  <head>
    <meta charset="utf-8">
    <title>Control TEA5767</title>
    <style>
      :root { font-family: Arial, Helvetica, sans-serif; }
      body { text-align:center; margin: 32px auto; max-width: 900px; }
      h1 { margin-bottom: 8px; }
      h2 { margin: 28px 0 12px; }
      .grid {
        display: grid;
        grid-template-columns: repeat(auto-fit, minmax(160px, 1fr));
        gap: 12px;
        align-items: stretch;
        justify-items: center;
      }
      button {
        width: 100%; height: 60px; font-size: 18px; margin: 0;
        cursor: pointer;
      }
      .pot-btn {
        width: 40px;       /* antes 100% */
        height: 40px;      /* antes 60px */
        font-size: 14px;   /* antes 18px */
        padding: 4px;
      }
      .pot-row {
        display: flex;
        gap: 6px;             /* separación entre botones */
        justify-content: center;
        align-items: center;
      }
      .pot-input {
        width: 60px;
        height: 32px;
        font-size: 14px;
      }
      .card {
        border: 1px solid #ddd; border-radius: 10px; padding: 16px; text-align: left;
      }
      .rows { display: grid; gap: 8px; }
      .row { display: flex; justify-content: space-between; }
      .key { color: #555; }
      .val { font-weight: bold; }
      .hidden { display: none; }
      .statusbar { margin-top: 10px; color: #666; font-size: 14px; }

      .footer {
        margin-top: 24px;
        text-align: center;
        font-size: 14px;
        color: #444;
        line-height: 1.4;
      }
      .footer .title { font-weight: bold; font-size: 16px; }


    </style>
    <script>
      // -------- Utilidad para enviar pulsos a endpoints --------
      function enviarPulso(pin) {
        fetch('/' + pin, { method: 'GET' })
          .then(r => r.text())
          .then(t => {
            console.log('OK:', pin, t);
            cargarEstadoTEA(); 
            cargarEstadoPOT();
          })
          .catch(err => console.error('ERR:', pin, err));
      }

      function enviarValor() {
        const raw = document.getElementById('freqInput').value;
        if (!raw) { alert('Ingrese un número primero'); return; }

        const norm = raw.replace(',', '.');

        fetch('/set_freq?v=' + encodeURIComponent(norm))
          .then(r => r.text())
          .then(t => {
            console.log('Respuesta del servidor:', t);
            cargarEstadoTEA();
          })
          .catch(err => console.error('Error al enviar valor:', err));
      }

      function enviarVolumen() {
        const raw = document.getElementById('volInput').value;
        if (!raw) { alert('Ingrese un número primero'); return; }

        fetch('/pot_set?v=' + encodeURIComponent(raw))
          .then(r => r.text())
          .then(t => {
            console.log('Respuesta del servidor:', t);
            cargarEstadoPOT();
          })
          .catch(err => console.error('Error al enviar valor:', err));
      }

      // -------- Potenciómetro digital: estado y render --------
      function renderPOT(state) {
        const volEl = document.getElementById('pot-volume');
        const v = Number(state.volume);
        if (Number.isFinite(v)) {
          volEl.textContent = v + ' %';
        } else {
          volEl.textContent = '—';
        }
      }

      function cargarEstadoPOT() {
        // Endpoint esperado: GET /pot/status -> { volume: int }
        return fetch('/pot/status')
          .then(r => r.json())
          .then(json => {
            renderPOT(json);
            document.getElementById('pot-status').textContent = 'Volumen actualizado';
          })
          .catch(err => {
            console.error('No se pudo obtener /pot/status', err);
            document.getElementById('pot-status').textContent = 'Error al actualizar volumen';
          });
      }

      // -------- Estado TEA5767 --------
      function renderTEA(state) {
        const standbyEl   = document.getElementById('tea-standby');
        const freqValEl   = document.getElementById('tea-frequency');
        const muteValEl   = document.getElementById('tea-mute');
        const qualValEl   = document.getElementById('tea-qual');
        const volEl       = document.getElementById('pot-volume'); // volumen del pote
        const detailsWrap = document.getElementById('tea-details');
        const volRow      = document.getElementById('vol-row');

        const standby = !!state.standby;
        const mute    = !!state.mute;

        standbyEl.textContent = standby ? 'Pausado' : 'Activo';

        if (!standby) {
          detailsWrap.classList.remove('hidden');

          const f = Number(state.frequency);
          const q = Number(state.quality);

          freqValEl.textContent = Number.isFinite(f) ? f.toFixed(2) + ' MHz' : '—';
          qualValEl.textContent = Number.isFinite(q) ? q : '—';
          muteValEl.textContent = mute ? 'Silencio' : 'Sonando';
          if (mute) {
            volRow.classList.add('hidden');   // ocultar solo volumen
          } else {
            volRow.classList.remove('hidden'); // mostrar volumen
          }
          // el volumen lo completa cargarEstadoPOT() como siempre
        } else {
          detailsWrap.classList.add('hidden');

          freqValEl.textContent = '—';
          muteValEl.textContent = '—';
          qualValEl.textContent = '—';
          volRow.classList.add('hidden'); 
        }
      }

      function cargarEstadoTEA() {
        // Endpoint esperado: GET /tea/status -> { standby: bool, frequency: number, mute: bool, quality: number }
        return fetch('/tea/status')
          .then(r => r.json())
          .then(json => {
            renderTEA(json);
            document.getElementById('status-msg').textContent = 'Estado actualizado correctamente';
          })
          .catch(err => {
            console.error('No se pudo obtener /tea/status', err);
            document.getElementById('status-msg').textContent = 'Error al actualizar estado';
          });
      }

      // Auto-actualización
      let teaTimer = null;
      window.addEventListener('DOMContentLoaded', () => {
        cargarEstadoTEA();
        cargarEstadoPOT();
        teaTimer = setInterval(cargarEstadoTEA, 500);
      });

    </script>
  </head>
  <footer class="card footer">
    <div class="title">Taller de Proyecto 2</div>
    <div>Facultad de Informática - UNLP</div>
    <div>Grupo F5 - Prof. Fernando Tinetti</div>
    <div>Autores: Ramiro Madera y Ulises Pereira</div>
  </footer>
  <body>

    <h1>Control TEA5767</h1>
    <div class="grid" aria-label="Controles TEA5767">
      <!-- Ajustá los endpoints a tu firmware -->
      <button onclick="enviarPulso('_up')">Seek ↑</button>
      <button onclick="enviarPulso('_down')">Seek ↓</button>
      <button onclick="enviarPulso('mute_on_off')">Mute</button>
      <button onclick="enviarPulso('stand_by')">Standby ON/OFF</button>
      <div style="margin-top: 0px;">
        <button onclick="enviarValor()">Enviar valor</button>
        <input id="freqInput" type="number" step="0.1" inputmode="decimal"
              placeholder="Ej: 101.9" style="width:120px; height:30px; font-size:16px;">
      </div>
    </div>

    <h2>Potenciómetro digital</h2>
      <div class="pot-row">
        <button class="pot-btn" onclick="enviarPulso('pot_dec10')">-10</button>
        <button class="pot-btn" onclick="enviarPulso('pot_dec5')">-5</button>
        <button class="pot-btn" onclick="enviarPulso('pot_dec1')">-1</button>

        <div style="margin-top: 0px;">
          <button onclick="enviarVolumen()">Volumen</button>
          <input id="volInput" class="pot-input">
        </div>

        <button class="pot-btn" onclick="enviarPulso('pot_inc1')">+1</button>
        <button class="pot-btn" onclick="enviarPulso('pot_inc5')">+5</button>
        <button class="pot-btn" onclick="enviarPulso('pot_inc10')">+10</button>
        
      </div>


      


    <div class="card" style="margin-top:16px;">
      <div class="rows">
        <div class="row">
          <span class="key">Standby:</span>
          <span class="val" id="tea-standby">—</span>
        </div>

        <div id="tea-details" class="rows hidden">
          <div class="row" id="tea-freq-row">
            <span class="key">Frecuencia:</span>
            <span class="val" id="tea-frequency">—</span>
          </div>
          <div class="row" id="tea-qual-row">
            <span class="key">Calidad:</span>
            <span class="val" id="tea-qual">—</span>
          </div>
          <div class="row" id="tea-mute-row">
            <span class="key">Mute:</span>
            <span class="val" id="tea-mute">—</span>
          </div>
        </div>

        <!-- VOLUMEN: separado para controlar visibilidad independiente -->
        <div class="row" id="vol-row">
          <span class="key">Volumen:</span>
          <span class="val" id="pot-volume">—</span>
        </div>

      </div>
      <div class="statusbar" id="status-msg">—</div>
    </div>
  </body>
</html>
)rawliteral";
