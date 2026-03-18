#include <arduino.h>

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
  <head>
    <meta charset="utf-8">
    <title>Control HEX3653 & TEA5767</title>
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
            cargarEstadoTEA(); // 🔁 actualiza inmediatamente después del clic
          })
          .catch(err => console.error('ERR:', pin, err));
      }
      function enviarValor() {
        const raw = document.getElementById('valorEntero').value;
        if (!raw) { alert('Ingrese un número primero'); return; }

        // Normalizo decimal: 101,9 -> 101.9
        const norm = raw.replace(',', '.');

        fetch('/set_freq?v=' + encodeURIComponent(norm))   // <-- ruta y parámetro como en el backend
          .then(r => r.text())
          .then(t => {
            console.log('Respuesta del servidor:', t);
            cargarEstadoTEA();
          })
          .catch(err => console.error('Error al enviar valor:', err));
      }



      // -------- Estado TEA5767 --------
      function renderTEA(state) {
        const standbyEl   = document.getElementById('tea-standby');
        const freqRowEl   = document.getElementById('tea-freq-row');
        const muteRowEl   = document.getElementById('tea-mute-row');
        const qualRowEl   = document.getElementById('tea-qual-row');
        const freqValEl   = document.getElementById('tea-frequency');
        const muteValEl   = document.getElementById('tea-mute');
        const qualValEl   = document.getElementById('tea-qual');
        const detailsWrap = document.getElementById('tea-details');

        const standby = !!state.standby;
        standbyEl.textContent = standby ? 'Pausado' : 'Activo';

        // Mostrar/ocultar detalles según standby
        if (!standby) {
          detailsWrap.classList.remove('hidden');
          // Valores
          const f = Number(state.frequency);
          const q = state.quality;
          freqValEl.textContent = Number.isFinite(f) ? f.toFixed(2) + ' MHz' : '—';
          muteValEl.textContent = state.mute ? 'Silencio' : 'Sonando';
          qualValEl.textContent = q;
        } else {
          detailsWrap.classList.add('hidden');
          // Limpio por si quedaron viejos
          freqValEl.textContent = '—';
          muteValEl.textContent = '—';
          qualValEl.textContent = '—';
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
        teaTimer = setInterval(cargarEstadoTEA, 500);
        // Si querés desactivar el polling automático, comentá la línea anterior
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
    <h1>Control HEX3653</h1>
    <div class="grid" aria-label="Controles HEX3653">
      <button onclick="enviarPulso('s_up')">S_UP (D8)</button>
      <button onclick="enviarPulso('s_down')">S_DOWN (D7)</button>
      <button onclick="enviarPulso('v_up')">V_UP (D6)</button>
      <button onclick="enviarPulso('v_down')">V_DOWN (D5)</button>
      <button onclick="enviarPulso('on_off')">ON/OFF (D0)</button>
    </div>

    <h1>Control TEA5767</h1>
    <div class="grid" aria-label="Controles TEA5767">
      <!-- Ajustá los endpoints a tu firmware -->
      <button onclick="enviarPulso('_up')">Seek ↑</button>
      <button onclick="enviarPulso('_down')">Seek ↓</button>
      <button onclick="enviarPulso('mute_on_off')">Mute</button>
      <button onclick="enviarPulso('stand_by')">Standby ON/OFF</button>
      <div style="margin-top: 0px;">
        <button onclick="enviarValor()">Enviar valor</button>
        <input id="valorEntero" type="number" step="0.1" inputmode="decimal" placeholder="Ej: 101.9" style="width:120px; height:30px; font-size:16px;">
      </div>
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
          <div class="row" id="tea-mute-row">
            <span class="key">Mute:</span>
            <span class="val" id="tea-mute">—</span>
          </div>
          <div class="row" id="tea-qual-row">
            <span class="key">Calidad:</span>
            <span class="val" id="tea-qual">—</span>
          </div>
        </div>
      </div>
      <div class="statusbar" id="status-msg">—</div>
    </div>
  </body>
</html>
)rawliteral";
