#pragma once

#include <Arduino.h>

// Shared Brand Logo Data URL (128x128 Transparent White Cyber Logo)
#define ARK_LOGO_SRC "data:image/png;base64,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"

// =============================================================================
// CALIBRATION PAGE HTML (http://arkbot.local/calib)
// =============================================================================
const char CALIB_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no">
  <title>ARK-BOT Calibrator | Cyber Kinematics</title>
  <style>
    :root {
      --bg: #070a0f;
      --card-bg: #0f1523;
      --card-border: #1e293b;
      --text-main: #f1f5f9;
      --text-dim: #94a3b8;
      --cyan: #00f0ff;
      --cyan-glow: rgba(0, 240, 255, 0.4);
      --accent: #3b82f6;
      --purple: #a855f7;
      --green: #10b981;
      --green-glow: rgba(16, 185, 129, 0.4);
      --red: #ef4444;
      --red-glow: rgba(239, 68, 68, 0.4);
      --yellow: #f59e0b;
    }
    * { box-sizing: border-box; margin: 0; padding: 0; font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, sans-serif; }
    body { background-color: var(--bg); color: var(--text-main); padding: 12px; max-width: 1000px; margin: 0 auto; padding-bottom: 50px; }
    
    /* Header */
    header { display: flex; align-items: center; justify-content: space-between; padding: 12px 18px; background: var(--card-bg); border-radius: 14px; border: 1px solid var(--card-border); margin-bottom: 14px; box-shadow: 0 8px 24px rgba(0,0,0,0.4); flex-wrap: wrap; gap: 12px; }
    .brand { display: flex; align-items: center; gap: 14px; }
    .brand-logo { width: 44px; height: 44px; object-fit: contain; filter: drop-shadow(0 0 10px var(--cyan-glow)); transition: transform 0.25s ease, filter 0.25s ease; cursor: pointer; }
    .brand-logo:hover { transform: scale(1.08) rotate(3deg); filter: drop-shadow(0 0 18px rgba(0, 240, 255, 0.9)); }
    .title h1 { font-size: 20px; font-weight: 800; letter-spacing: 0.8px; background: linear-gradient(90deg, #ffffff, var(--cyan)); -webkit-background-clip: text; -webkit-text-fill-color: transparent; }
    .title p { font-size: 11px; color: var(--text-dim); }
    
    /* Nav Tabs */
    .nav-tabs { display: flex; gap: 6px; background: rgba(0,0,0,0.4); padding: 4px; border-radius: 10px; border: 1px solid var(--card-border); }
    .nav-tab { display: inline-flex; align-items: center; gap: 6px; padding: 6px 14px; border-radius: 7px; font-size: 12px; font-weight: 700; text-decoration: none; color: var(--text-dim); transition: all 0.2s ease; }
    .nav-tab:hover { color: var(--text-main); background: rgba(255,255,255,0.05); }
    .nav-tab.active { background: linear-gradient(135deg, rgba(0,240,255,0.2), rgba(59,130,246,0.3)); color: var(--cyan); border: 1px solid var(--cyan); box-shadow: 0 0 10px var(--cyan-glow); }

    .header-right { display: flex; align-items: center; gap: 10px; flex-wrap: wrap; }
    
    /* RF Antenna Widget */
    .rf-widget { display: flex; align-items: center; gap: 6px; background: rgba(0,0,0,0.35); padding: 4px 8px; border-radius: 10px; border: 1px solid var(--card-border); }
    .rf-label { font-size: 10px; font-weight: 700; color: var(--text-dim); text-transform: uppercase; letter-spacing: 0.5px; }
    .rf-btns { display: flex; gap: 3px; }
    .rf-btn { background: #182236; border: 1px solid rgba(255,255,255,0.06); color: var(--text-dim); padding: 4px 8px; border-radius: 6px; font-size: 11px; font-weight: 700; cursor: pointer; transition: all 0.15s ease; }
    .rf-btn:hover { color: #fff; border-color: var(--cyan); }
    .rf-btn.active { background: linear-gradient(135deg, var(--cyan), var(--accent)); color: #000; font-weight: 800; border: none; box-shadow: 0 0 10px var(--cyan-glow); }
    .rssi-badge { font-size: 10px; font-weight: 700; color: #34d399; background: rgba(16, 185, 129, 0.12); padding: 2px 6px; border-radius: 4px; border: 1px solid rgba(16, 185, 129, 0.3); }

    .badge { padding: 5px 12px; border-radius: 20px; font-size: 11px; font-weight: 700; text-transform: uppercase; letter-spacing: 0.5px; }
    .badge-online { background: rgba(16, 185, 129, 0.15); color: #34d399; border: 1px solid #059669; box-shadow: 0 0 10px rgba(16, 185, 129, 0.2); }
    .badge-offline { background: rgba(239, 68, 68, 0.15); color: #f87171; border: 1px solid #dc2626; box-shadow: 0 0 10px rgba(239, 68, 68, 0.2); }

    /* Visual Simulation Canvas Card */
    .sim-card { background: radial-gradient(circle at center, #131b2e 0%, #0a0e18 100%); border: 1px solid var(--card-border); border-radius: 14px; padding: 14px; margin-bottom: 16px; position: relative; overflow: hidden; box-shadow: 0 12px 32px rgba(0,0,0,0.5); }
    .sim-topbar { display: flex; justify-content: space-between; align-items: center; margin-bottom: 10px; flex-wrap: wrap; gap: 8px; }
    .sim-title { font-size: 12px; font-weight: 700; text-transform: uppercase; color: var(--cyan); letter-spacing: 1px; display: flex; align-items: center; gap: 8px; }
    .view-toggles { display: flex; gap: 6px; background: rgba(0,0,0,0.3); padding: 3px; border-radius: 8px; border: 1px solid var(--card-border); }
    .view-btn { background: transparent; border: none; color: var(--text-dim); padding: 4px 10px; border-radius: 6px; font-size: 11px; font-weight: 700; cursor: pointer; transition: all 0.15s ease; }
    .view-btn.active { background: var(--accent); color: #fff; box-shadow: 0 0 8px var(--cyan-glow); }
    
    .canvas-container { position: relative; width: 100%; height: 340px; border-radius: 10px; background: #070a12; border: 1px solid rgba(255,255,255,0.05); overflow: hidden; }
    canvas { width: 100%; height: 100%; display: block; }
    
    .sim-footer { display: flex; justify-content: space-between; align-items: center; margin-top: 10px; font-size: 11px; color: var(--text-dim); flex-wrap: wrap; gap: 8px; }
    .sim-legend { display: flex; gap: 14px; }
    .legend-item { display: flex; align-items: center; gap: 6px; }
    .legend-dot { width: 8px; height: 8px; border-radius: 50%; }

    /* Master Controls */
    .section-title { font-size: 12px; font-weight: 700; text-transform: uppercase; color: var(--text-dim); margin-bottom: 8px; letter-spacing: 0.8px; display: flex; justify-content: space-between; align-items: center; }
    .master-grid { display: grid; grid-template-columns: repeat(auto-fit, minmax(140px, 1fr)); gap: 8px; margin-bottom: 18px; }
    
    .btn { background: #131a29; color: var(--text-main); border: 1px solid var(--card-border); padding: 10px 14px; border-radius: 10px; font-size: 13px; font-weight: 700; cursor: pointer; transition: all 0.15s ease; display: inline-flex; align-items: center; justify-content: center; gap: 6px; user-select: none; }
    .btn:active { transform: scale(0.97); }
    .btn-green { background: linear-gradient(135deg, #059669, #10b981); border: none; color: #fff; box-shadow: 0 0 12px var(--green-glow); }
    .btn-red { background: linear-gradient(135deg, #b91c1c, #ef4444); border: none; color: #fff; box-shadow: 0 0 12px var(--red-glow); }
    .btn-cyan { background: linear-gradient(135deg, #00f0ff, #3b82f6); border: none; color: #000; font-weight: 800; box-shadow: 0 0 14px var(--cyan-glow); }
    .btn-dark { background: #182235; border: 1px solid #2d3b55; }
    .btn-dark:hover { background: #202c44; border-color: var(--cyan); }

    /* Leg Grid */
    .legs-grid { display: grid; grid-template-columns: 1fr; gap: 14px; }
    @media(min-width: 720px) { .legs-grid { grid-template-columns: 1fr 1fr; } }
    
    .leg-card { background: var(--card-bg); border: 1px solid var(--card-border); border-radius: 14px; padding: 16px; transition: all 0.2s ease; box-shadow: 0 6px 20px rgba(0,0,0,0.3); }
    .leg-card.disabled-card { opacity: 0.6; border-color: rgba(255,255,255,0.05); }
    .leg-card.card-highlight { border-color: var(--cyan); box-shadow: 0 0 16px var(--cyan-glow); }
    
    .leg-header { display: flex; justify-content: space-between; align-items: center; margin-bottom: 14px; padding-bottom: 8px; border-bottom: 1px solid rgba(255,255,255,0.06); }
    .leg-title { font-size: 15px; font-weight: 800; color: var(--cyan); display: flex; align-items: center; gap: 8px; letter-spacing: 0.5px; }
    .leg-actions { display: flex; gap: 6px; }

    /* Joint Control */
    .joint-row { margin-bottom: 12px; background: #131b2c; padding: 10px 12px; border-radius: 10px; border: 1px solid rgba(255,255,255,0.04); transition: border-color 0.2s; }
    .joint-row:last-child { margin-bottom: 0; }
    .joint-row.joint-disabled { opacity: 0.5; }
    .joint-row:hover { border-color: rgba(0,240,255,0.3); }

    .joint-top { display: flex; justify-content: space-between; align-items: center; margin-bottom: 8px; font-size: 13px; }
    .joint-left { display: flex; align-items: center; gap: 8px; }
    .joint-name { font-weight: 700; color: var(--text-main); }
    .joint-ch { font-size: 10px; font-weight: 700; color: var(--cyan); background: #1b263b; padding: 2px 7px; border-radius: 4px; border: 1px solid rgba(0,240,255,0.2); }
    
    .joint-right { display: flex; align-items: center; gap: 8px; }
    .joint-val { font-family: ui-monospace, SFMono-Regular, Menlo, Monaco, Consolas, monospace; font-weight: 800; color: var(--cyan); background: #090d16; padding: 3px 8px; border-radius: 6px; border: 1px solid #1e293b; font-size: 13px; min-width: 48px; text-align: center; }

    /* Power Switch Toggle */
    .switch-btn { padding: 4px 9px; font-size: 11px; font-weight: 700; border-radius: 6px; cursor: pointer; border: 1px solid var(--card-border); background: #1a2233; color: var(--text-dim); transition: all 0.15s ease; }
    .switch-btn.on { background: rgba(16, 185, 129, 0.2); color: #34d399; border-color: #059669; }
    .switch-btn.off { background: rgba(239, 68, 68, 0.2); color: #f87171; border-color: #dc2626; }

    /* Slider Container */
    .slider-container { display: flex; align-items: center; gap: 8px; margin-top: 4px; }
    input[type=range] { flex: 1; height: 6px; border-radius: 3px; background: #243048; outline: none; -webkit-appearance: none; }
    input[type=range]::-webkit-slider-thumb { -webkit-appearance: none; appearance: none; width: 22px; height: 22px; border-radius: 50%; background: var(--cyan); cursor: pointer; box-shadow: 0 0 10px var(--cyan-glow); }
    input[type=range]:disabled::-webkit-slider-thumb { background: #475569; box-shadow: none; cursor: not-allowed; }
    
    .quick-btns { display: flex; gap: 3px; }
    .btn-sm { padding: 5px 7px; font-size: 10px; font-weight: 700; border-radius: 5px; min-width: 32px; background: #1a2336; border: 1px solid var(--card-border); color: var(--text-dim); cursor: pointer; }
    .btn-sm:hover:not(:disabled) { background: #26344f; color: var(--text-main); }
    .btn-sm:disabled { opacity: 0.4; cursor: not-allowed; }

    /* Footer */
    .app-footer { margin-top: 36px; padding-top: 22px; border-top: 1px solid rgba(255, 255, 255, 0.08); text-align: center; }
    .footer-inner { display: flex; flex-direction: column; align-items: center; gap: 6px; }
    .footer-brand { display: flex; align-items: center; gap: 8px; font-size: 11px; font-weight: 700; color: var(--text-dim); letter-spacing: 1.2px; text-transform: uppercase; }
    .footer-dot { width: 6px; height: 6px; border-radius: 50%; background: var(--cyan); box-shadow: 0 0 8px var(--cyan-glow); }
    .footer-credit { font-size: 13px; color: #cbd5e1; font-weight: 500; }
    .author-name { font-weight: 800; color: #ffffff; background: linear-gradient(90deg, #ffffff, var(--cyan)); -webkit-background-clip: text; -webkit-text-fill-color: transparent; }
    .footer-org { font-size: 11px; color: var(--text-dim); letter-spacing: 0.5px; }

    /* Toast Notification */
    #toast { position: fixed; bottom: 24px; left: 50%; transform: translateX(-50%); background: #10b981; color: white; padding: 10px 22px; border-radius: 30px; font-size: 13px; font-weight: 700; box-shadow: 0 8px 24px rgba(0,0,0,0.6); opacity: 0; pointer-events: none; transition: opacity 0.2s ease; z-index: 100; border: 1px solid rgba(255,255,255,0.2); }
    #toast.show { opacity: 1; }
  </style>
</head>
<body>

  <header>
    <div class="brand">
      <img src=")rawliteral" ARK_LOGO_SRC R"rawliteral(" class="brand-logo" alt="ARK-BOT Logo" onclick="location.href='/calib'">
      <div class="title">
        <h1>ARK-BOT</h1>
        <p>Cyber Kinematics Engine (ESP32-C6 + PCA9685)</p>
      </div>
    </div>

    <!-- Navigation Tabs -->
    <nav class="nav-tabs">
      <a href="/calib" class="nav-tab active">🎯 Calibrator</a>
      <a href="/setup" class="nav-tab">⚙️ Setup</a>
    </nav>
    
    <div class="header-right">
      <!-- Antenna Control -->
      <div class="rf-widget">
        <span class="rf-label">Antenna:</span>
        <div class="rf-btns">
          <button class="rf-btn active" id="btnAntInt" onclick="setAntenna('internal')" title="Switch to Onboard Ceramic Antenna">📡 Int</button>
          <button class="rf-btn" id="btnAntExt" onclick="setAntenna('external')" title="Switch to External IPEX/U.FL Antenna">🛰️ Ext</button>
        </div>
        <span class="rssi-badge" id="rssiVal">📶 -- dBm</span>
      </div>
      
      <div id="pcaStatus" class="badge badge-online">PCA: CHECKING</div>
    </div>
  </header>

  <!-- Visual Animated Robot Simulation Card -->
  <div class="sim-card">
    <div class="sim-topbar">
      <div class="sim-title">
        <span>⚡ Physical Kinematics View</span>
      </div>
      <div class="view-toggles">
        <button class="view-btn active" id="btnViewIso" onclick="setViewMode('iso')">3D Isometric</button>
        <button class="view-btn" id="btnViewTop" onclick="setViewMode('top')">Top-Down</button>
        <button class="view-btn" id="btnViewSide" onclick="setViewMode('side')">Side Elevation</button>
      </div>
    </div>
    
    <div class="canvas-container">
      <canvas id="robotCanvas"></canvas>
    </div>

    <div class="sim-footer">
      <div class="sim-legend">
        <div class="legend-item"><span class="legend-dot" style="background:var(--cyan)"></span> Active Joint</div>
        <div class="legend-item"><span class="legend-dot" style="background:var(--red)"></span> Released (0V)</div>
        <div class="legend-item"><span class="legend-dot" style="background:var(--green)"></span> Ground Contact</div>
      </div>
      <div id="activeCount" style="color:var(--cyan); font-weight:700;">12 / 12 Servos Energized</div>
    </div>
  </div>

  <!-- Master Controls -->
  <div class="section-title">Master Power & Alignment</div>
  <div class="master-grid">
    <button class="btn btn-green" onclick="setMasterPower(1)">⚡ Start All Servos</button>
    <button class="btn btn-red" onclick="setMasterPower(0)">🛑 Stop All (Release)</button>
    <button class="btn btn-cyan" onclick="sendInit('all')">🎯 Center All (90°)</button>
    <button class="btn btn-dark" onclick="sendInit('wave')">🌊 Wave Calibration</button>
    <button class="btn btn-dark" onclick="sendBeep()">🔔 Beep Buzzer</button>
  </div>

  <!-- 4 Leg Control Cards -->
  <div class="section-title">Individual Leg & Joint Controls</div>
  <div class="legs-grid">
    <div class="leg-card" id="legCard0"></div>
    <div class="leg-card" id="legCard1"></div>
    <div class="leg-card" id="legCard2"></div>
    <div class="leg-card" id="legCard3"></div>
  </div>

  <!-- App Footer -->
  <footer class="app-footer">
    <div class="footer-inner">
      <div class="footer-brand">
        <span class="footer-dot"></span>
        <span class="footer-title">ARK-BOT SYSTEM &bull; v0.1.2</span>
      </div>
      <div class="footer-credit">
        Designed & Engineered by <span class="author-name">Amuthesan</span>
      </div>
      <div class="footer-org">Ark Technology</div>
    </div>
  </footer>

  <div id="toast">Command Sent</div>

  <script>
    const LEGS = ["Front-Right (FR)", "Front-Left (FL)", "Rear-Right (RR)", "Rear-Left (RL)"];
    const SHORT_LEGS = ["FR", "FL", "RR", "RL"];
    const JOINTS = ["Coxa (Hip)", "Femur (Thigh)", "Tibia (Calf)"];
    const CHANNELS = [
      [4, 2, 3],    // FR: Coxa CH4, Femur CH2, Tibia CH3
      [10, 8, 9],   // FL: Coxa CH10, Femur CH8, Tibia CH9
      [7, 5, 6],    // RR: Coxa CH7, Femur CH5, Tibia CH6
      [12, 11, 13]  // RL: Coxa CH12, Femur CH11, Tibia CH13
    ];
    
    let targetAngles = Array(4).fill().map(() => Array(3).fill(90));
    let currentSimAngles = Array(4).fill().map(() => Array(3).fill(90));
    let servoPower = Array(4).fill().map(() => Array(3).fill(true));
    let viewMode = 'iso'; // 'iso', 'top', 'side'
    let waveAnimPhase = 0;
    let isExternalAntenna = false;

    // Load Bot Logo for Canvas
    const botLogoImg = new Image();
    botLogoImg.src = ")rawliteral" ARK_LOGO_SRC R"rawliteral(";

    function setViewMode(mode) {
      viewMode = mode;
      ['Iso', 'Top', 'Side'].forEach(m => {
        const b = document.getElementById(`btnView${m}`);
        if (b) b.className = (m.toLowerCase() === mode) ? 'view-btn active' : 'view-btn';
      });
    }

    // Build the 4 Leg Control Cards
    function buildCards() {
      for (let l = 0; l < 4; l++) {
        const card = document.getElementById(`legCard${l}`);
        let html = `
          <div class="leg-header">
            <div class="leg-title">${LEGS[l]}</div>
            <div class="leg-actions">
              <button class="switch-btn on" id="btnLegPower_${l}" onclick="toggleLegPower(${l})">PWR ON</button>
              <button class="btn btn-sm" onclick="initLeg(${l})">Init 90°</button>
            </div>
          </div>
        `;
        for (let j = 0; j < 3; j++) {
          const ch = CHANNELS[l][j];
          html += `
            <div class="joint-row" id="jointRow_${l}_${j}">
              <div class="joint-top">
                <div class="joint-left">
                  <span class="joint-name">${JOINTS[j]}</span>
                  <span class="joint-ch">CH ${ch}</span>
                </div>
                <div class="joint-right">
                  <button class="switch-btn on" id="btnPwr_${l}_${j}" onclick="toggleJointPower(${l}, ${j})">ON</button>
                  <div class="joint-val" id="val_${l}_${j}">90°</div>
                </div>
              </div>
              <div class="slider-container">
                <input type="range" id="slider_${l}_${j}" min="0" max="180" value="90" 
                  oninput="onSliderInput(${l}, ${j}, this.value)"
                  onchange="onSliderChange(${l}, ${j}, this.value)">
                <div class="quick-btns">
                  <button class="btn-sm" id="btn0_${l}_${j}" onclick="setJoint(${l}, ${j}, 0)">0°</button>
                  <button class="btn-sm" id="btn90_${l}_${j}" onclick="setJoint(${l}, ${j}, 90)">90°</button>
                  <button class="btn-sm" id="btn180_${l}_${j}" onclick="setJoint(${l}, ${j}, 180)">180°</button>
                  <button class="btn-sm" id="btnInit_${l}_${j}" style="color:var(--cyan)" title="Init this joint" onclick="initSingleJoint(${l}, ${j})">⚡</button>
                </div>
              </div>
            </div>
          `;
        }
        card.innerHTML = html;
      }
    }

    let throttleTimer = null;
    function onSliderInput(leg, joint, angle) {
      document.getElementById(`val_${leg}_${joint}`).innerText = `${angle}°`;
      targetAngles[leg][joint] = parseInt(angle);
      
      if (!servoPower[leg][joint]) {
        updateSinglePowerUI(leg, joint, true);
      }
      
      if (!throttleTimer) {
        throttleTimer = setTimeout(() => {
          sendServoAngle(leg, joint, angle);
          throttleTimer = null;
        }, 50);
      }
    }

    function onSliderChange(leg, joint, angle) {
      document.getElementById(`val_${leg}_${joint}`).innerText = `${angle}°`;
      targetAngles[leg][joint] = parseInt(angle);
      sendServoAngle(leg, joint, angle);
    }

    function setJoint(leg, joint, angle) {
      document.getElementById(`slider_${leg}_${joint}`).value = angle;
      document.getElementById(`val_${leg}_${joint}`).innerText = `${angle}°`;
      targetAngles[leg][joint] = parseInt(angle);
      sendServoAngle(leg, joint, angle);
    }

    function initSingleJoint(leg, joint) {
      setJoint(leg, joint, 90);
      showToast(`Initialized Leg ${SHORT_LEGS[leg]} ${JOINTS[joint]} to 90°`);
    }

    function initLeg(leg) {
      for (let j = 0; j < 3; j++) {
        setJoint(leg, j, 90);
      }
      showToast(`Initialized Leg ${SHORT_LEGS[leg]} (all 3 joints 90°)`);
    }

    async function setAntenna(type) {
      showToast(`Switching to ${type.toUpperCase()} Antenna...`);
      try {
        const res = await fetch(`/api/antenna?type=${type}`, { method: 'POST' });
        const data = await res.json();
        updateAntennaUI(data.extAntenna, data.rssi);
        showToast(`Antenna switched: ${data.extAntenna ? 'EXTERNAL (IPEX)' : 'INTERNAL (Ceramic)'}`);
      } catch (err) {
        console.error("Antenna API error", err);
      }
    }

    function updateAntennaUI(isExt, rssi) {
      isExternalAntenna = isExt;
      const btnInt = document.getElementById('btnAntInt');
      const btnExt = document.getElementById('btnAntExt');
      const rssiBadge = document.getElementById('rssiVal');
      
      if (btnInt && btnExt) {
        btnInt.className = isExt ? 'rf-btn' : 'rf-btn active';
        btnExt.className = isExt ? 'rf-btn active' : 'rf-btn';
      }
      if (rssiBadge && rssi !== undefined) {
        rssiBadge.innerText = `📶 ${rssi} dBm`;
      }
    }

    async function setMasterPower(state) {
      showToast(state ? "Energizing All Servos..." : "Releasing / Stopping All Servos...");
      try {
        await fetch(`/api/power?target=all&state=${state}`, { method: 'POST' });
        for (let l = 0; l < 4; l++) {
          for (let j = 0; j < 3; j++) {
            updateSinglePowerUI(l, j, state === 1);
          }
          updateLegHeaderPowerUI(l);
        }
      } catch (err) {
        console.error("Master power error", err);
      }
    }

    async function toggleLegPower(leg) {
      const anyOn = servoPower[leg].some(p => p);
      const newState = !anyOn;
      try {
        await fetch(`/api/power?target=leg&leg=${leg}&state=${newState ? 1 : 0}`, { method: 'POST' });
        for (let j = 0; j < 3; j++) {
          updateSinglePowerUI(leg, j, newState);
        }
        updateLegHeaderPowerUI(leg);
        showToast(`Leg ${SHORT_LEGS[leg]} Power: ${newState ? 'ON' : 'OFF'}`);
      } catch (err) {
        console.error("Leg power error", err);
      }
    }

    async function toggleJointPower(leg, joint) {
      const newState = !servoPower[leg][joint];
      try {
        await fetch(`/api/power?target=joint&leg=${leg}&joint=${joint}&state=${newState ? 1 : 0}`, { method: 'POST' });
        updateSinglePowerUI(leg, joint, newState);
        updateLegHeaderPowerUI(leg);
        showToast(`Leg ${SHORT_LEGS[leg]} Joint ${joint} ${newState ? 'ON' : 'OFF'}`);
      } catch (err) {
        console.error("Joint power error", err);
      }
    }

    function updateSinglePowerUI(leg, joint, isOn) {
      servoPower[leg][joint] = isOn;
      const btn = document.getElementById(`btnPwr_${leg}_${joint}`);
      const row = document.getElementById(`jointRow_${leg}_${joint}`);
      const slider = document.getElementById(`slider_${leg}_${joint}`);
      if (btn) {
        btn.className = `switch-btn ${isOn ? 'on' : 'off'}`;
        btn.innerText = isOn ? 'ON' : 'OFF';
      }
      if (row) {
        if (isOn) row.classList.remove('joint-disabled');
        else row.classList.add('joint-disabled');
      }
      if (slider) slider.disabled = !isOn;
      ['0', '90', '180', 'Init'].forEach(k => {
        const b = document.getElementById(`btn${k}_${leg}_${joint}`);
        if (b) b.disabled = !isOn;
      });
    }

    function updateLegHeaderPowerUI(leg) {
      const anyOn = servoPower[leg].some(p => p);
      const legBtn = document.getElementById(`btnLegPower_${leg}`);
      const card = document.getElementById(`legCard${leg}`);
      if (legBtn) {
        legBtn.className = `switch-btn ${anyOn ? 'on' : 'off'}`;
        legBtn.innerText = anyOn ? 'PWR ON' : 'PWR OFF';
      }
      if (card) {
        if (anyOn) card.classList.remove('disabled-card');
        else card.classList.add('disabled-card');
      }
      
      let activeCount = 0;
      for (let l = 0; l < 4; l++) {
        for (let j = 0; j < 3; j++) {
          if (servoPower[l][j]) activeCount++;
        }
      }
      const countEl = document.getElementById('activeCount');
      if (countEl) countEl.innerText = `${activeCount} / 12 Servos Energized`;
    }

    async function sendServoAngle(leg, joint, angle) {
      try {
        await fetch(`/api/servo?leg=${leg}&joint=${joint}&angle=${angle}`, { method: 'POST' });
      } catch (err) {
        console.error("Set servo error", err);
      }
    }

    async function sendInit(type) {
      showToast(type === 'wave' ? "Starting Wave Calibration..." : "Centering All 12 Servos to 90°...");
      try {
        const res = await fetch(`/api/init?type=${type}`, { method: 'POST' });
        const data = await res.json();
        updateUIWithStatus(data);
      } catch (err) {
        console.error("Init error", err);
      }
    }

    async function sendBeep() {
      try {
        await fetch('/api/beep', { method: 'POST' });
        showToast("Buzzer Beeped 🔔");
      } catch (err) {
        console.error("Beep error", err);
      }
    }

    async function fetchStatus() {
      try {
        const res = await fetch('/api/status');
        const data = await res.json();
        updateUIWithStatus(data);
      } catch (err) {
        console.error("Fetch status error", err);
        const badge = document.getElementById('pcaStatus');
        if (badge) {
          badge.className = 'badge badge-offline';
          badge.innerText = 'OFFLINE';
        }
      }
    }

    function updateUIWithStatus(data) {
      const badge = document.getElementById('pcaStatus');
      if (badge) {
        if (data.pcaReady) {
          badge.className = 'badge badge-online';
          badge.innerText = 'PCA: READY (0x40)';
        } else {
          badge.className = 'badge badge-offline';
          badge.innerText = 'PCA: NOT FOUND';
        }
      }

      if (data.extAntenna !== undefined) {
        updateAntennaUI(data.extAntenna, data.rssi);
      }

      if (data.angles) {
        for (let l = 0; l < 4; l++) {
          for (let j = 0; j < 3; j++) {
            const angle = data.angles[l][j];
            targetAngles[l][j] = angle;
            
            const slider = document.getElementById(`slider_${l}_${j}`);
            const val = document.getElementById(`val_${l}_${j}`);
            if (slider && document.activeElement !== slider) {
              slider.value = angle;
            }
            if (val) {
              val.innerText = `${angle}°`;
            }
          }
        }
      }

      if (data.enabled) {
        for (let l = 0; l < 4; l++) {
          for (let j = 0; j < 3; j++) {
            updateSinglePowerUI(l, j, data.enabled[l][j]);
          }
          updateLegHeaderPowerUI(l);
        }
      }
    }

    function showToast(msg) {
      const toast = document.getElementById('toast');
      toast.innerText = msg;
      toast.classList.add('show');
      setTimeout(() => toast.classList.remove('show'), 2000);
    }

    // =========================================================================
    // 3D/2D Kinematics Renderer
    // =========================================================================
    const canvas = document.getElementById('robotCanvas');
    const ctx = canvas.getContext('2d');

    function resizeCanvas() {
      const rect = canvas.parentElement.getBoundingClientRect();
      canvas.width = rect.width * window.devicePixelRatio;
      canvas.height = rect.height * window.devicePixelRatio;
      ctx.scale(window.devicePixelRatio, window.devicePixelRatio);
    }
    window.addEventListener('resize', resizeCanvas);
    resizeCanvas();

    function renderKinematics() {
      const w = canvas.parentElement.clientWidth;
      const h = canvas.parentElement.clientHeight;
      ctx.clearRect(0, 0, w, h);

      waveAnimPhase += 0.04;

      // Smooth mechanical LERP interpolation towards target angles
      for (let l = 0; l < 4; l++) {
        for (let j = 0; j < 3; j++) {
          currentSimAngles[l][j] += (targetAngles[l][j] - currentSimAngles[l][j]) * 0.22;
        }
      }

      const cx = w / 2;
      const cy = h / 2 + (viewMode === 'side' ? 20 : 0);

      // Cyber Grid Background Floor
      ctx.strokeStyle = "rgba(0, 240, 255, 0.05)";
      ctx.lineWidth = 1;
      const gridSize = 28;
      for (let x = 0; x < w; x += gridSize) {
        ctx.beginPath(); ctx.moveTo(x, 0); ctx.lineTo(x, h); ctx.stroke();
      }
      for (let y = 0; y < h; y += gridSize) {
        ctx.beginPath(); ctx.moveTo(0, y); ctx.lineTo(w, y); ctx.stroke();
      }

      // Robot Dimensions
      const bodyHalfW = 44;
      const bodyHalfL = 60;
      const bodyElevZ = 30;
      const coxaLen = 22;
      const femurLen = 42;
      const tibiaLen = 52;

      // Leg Mount Positions (FR, FL, RR, RL)
      const legMounts = [
        { sideX: 1, frontY: -1, name: "FR" },
        { sideX: -1, frontY: -1, name: "FL" },
        { sideX: 1, frontY: 1, name: "RR" },
        { sideX: -1, frontY: 1, name: "RL" }
      ];

      function projectPoint(x, y, z) {
        if (viewMode === 'top') {
          return { x: cx + x * 1.5, y: cy + y * 1.5 };
        } else if (viewMode === 'side') {
          return { x: cx + y * 1.7, y: cy - z * 1.5 };
        } else {
          // 3D Isometric View
          const isoX = (x - y) * Math.cos(Math.PI / 6);
          const isoY = (x + y) * Math.sin(Math.PI / 6) - z;
          return { x: cx + isoX * 1.25, y: cy + isoY * 1.25 + 10 };
        }
      }

      // Ground Shadow
      const shadowPoints = [];
      legMounts.forEach((m, l) => {
        const pGround = projectPoint(m.sideX * (bodyHalfW + coxaLen + 20), m.frontY * (bodyHalfL + 20), -35);
        shadowPoints.push(pGround);
      });
      ctx.beginPath();
      ctx.moveTo(shadowPoints[0].x, shadowPoints[0].y);
      for (let i = 1; i < shadowPoints.length; i++) ctx.lineTo(shadowPoints[i].x, shadowPoints[i].y);
      ctx.closePath();
      ctx.fillStyle = "rgba(0, 0, 0, 0.4)";
      ctx.fill();

      // Render 4 Legs
      legMounts.forEach((m, l) => {
        const coxaAng = (currentSimAngles[l][0] - 90) * (Math.PI / 180);
        const femurAng = (currentSimAngles[l][1] - 90) * (Math.PI / 180);
        const tibiaAng = (currentSimAngles[l][2] - 90) * (Math.PI / 180);

        const hipX = m.sideX * bodyHalfW;
        const hipY = m.frontY * bodyHalfL;
        const hipZ = bodyElevZ;

        const baseHeading = (m.sideX > 0) ? 0 : Math.PI;
        const coxaTheta = baseHeading + (m.sideX * coxaAng * 0.7);

        const kneeX = hipX + Math.cos(coxaTheta) * coxaLen;
        const kneeY = hipY + Math.sin(coxaTheta) * coxaLen;
        const kneeZ = hipZ;

        const fAng = -femurAng - 0.4;
        const ankleX = kneeX + Math.cos(coxaTheta) * (Math.cos(fAng) * femurLen);
        const ankleY = kneeY + Math.sin(coxaTheta) * (Math.cos(fAng) * femurLen);
        const ankleZ = kneeZ + Math.sin(fAng) * femurLen;

        const tAng = fAng - tibiaAng - 0.9;
        const footX = ankleX + Math.cos(coxaTheta) * (Math.cos(tAng) * tibiaLen);
        const footY = ankleY + Math.sin(coxaTheta) * (Math.cos(tAng) * tibiaLen);
        const footZ = ankleZ + Math.sin(tAng) * tibiaLen;

        const p0 = projectPoint(hipX, hipY, hipZ);
        const p1 = projectPoint(kneeX, kneeY, kneeZ);
        const p2 = projectPoint(ankleX, ankleY, ankleZ);
        const p3 = projectPoint(footX, footY, footZ);

        const isCoxaOn = servoPower[l][0];
        const isFemurOn = servoPower[l][1];
        const isTibiaOn = servoPower[l][2];

        // Coxa Segment
        ctx.beginPath();
        ctx.moveTo(p0.x, p0.y);
        ctx.lineTo(p1.x, p1.y);
        ctx.strokeStyle = isCoxaOn ? "#00f0ff" : "#ef4444";
        ctx.lineWidth = 5;
        ctx.lineCap = "round";
        ctx.stroke();

        // Femur Segment
        ctx.beginPath();
        ctx.moveTo(p1.x, p1.y);
        ctx.lineTo(p2.x, p2.y);
        ctx.strokeStyle = isFemurOn ? "#3b82f6" : "#ef4444";
        ctx.lineWidth = 4;
        ctx.stroke();

        // Tibia Segment
        ctx.beginPath();
        ctx.moveTo(p2.x, p2.y);
        ctx.lineTo(p3.x, p3.y);
        ctx.strokeStyle = isTibiaOn ? "#00f0ff" : "#ef4444";
        ctx.lineWidth = 3;
        ctx.stroke();

        // Joint Pivot Rings
        ctx.beginPath();
        ctx.arc(p0.x, p0.y, 6, 0, Math.PI * 2);
        ctx.fillStyle = isCoxaOn ? "#00f0ff" : "#ef4444";
        ctx.shadowColor = isCoxaOn ? "rgba(0,240,255,0.6)" : "transparent";
        ctx.shadowBlur = 8;
        ctx.fill();

        ctx.beginPath();
        ctx.arc(p1.x, p1.y, 5, 0, Math.PI * 2);
        ctx.fillStyle = isFemurOn ? "#3b82f6" : "#ef4444";
        ctx.fill();

        ctx.beginPath();
        ctx.arc(p2.x, p2.y, 4, 0, Math.PI * 2);
        ctx.fillStyle = isTibiaOn ? "#00f0ff" : "#ef4444";
        ctx.fill();

        // Ground Foot Contact Pad
        ctx.beginPath();
        ctx.arc(p3.x, p3.y, 6, 0, Math.PI * 2);
        const allOn = isCoxaOn && isFemurOn && isTibiaOn;
        ctx.fillStyle = allOn ? "#10b981" : "#ef4444";
        ctx.shadowColor = allOn ? "rgba(16,185,129,0.8)" : "transparent";
        ctx.shadowBlur = 10;
        ctx.fill();
        ctx.shadowBlur = 0;

        // Foot Dynamic Ground Contact Ripple
        if (allOn) {
          const rippleR = 6 + (Math.sin(waveAnimPhase * 2 + l) + 1) * 3;
          ctx.beginPath();
          ctx.arc(p3.x, p3.y, rippleR, 0, Math.PI * 2);
          ctx.strokeStyle = "rgba(16, 185, 129, 0.4)";
          ctx.lineWidth = 1.5;
          ctx.stroke();
        }

        // Leg Name Label Badge
        ctx.font = "bold 11px sans-serif";
        ctx.fillStyle = "#f1f5f9";
        ctx.fillText(`${m.name}`, p0.x + (m.sideX > 0 ? 12 : -30), p0.y + (m.frontY < 0 ? -8 : 14));
      });

      // Draw Main Chassis Body
      const c_fl = projectPoint(-bodyHalfW, -bodyHalfL, bodyElevZ + 10);
      const c_fr = projectPoint(bodyHalfW, -bodyHalfL, bodyElevZ + 10);
      const c_rr = projectPoint(bodyHalfW, bodyHalfL, bodyElevZ + 10);
      const c_rl = projectPoint(-bodyHalfW, bodyHalfL, bodyElevZ + 10);

      const b_fl = projectPoint(-bodyHalfW, -bodyHalfL, bodyElevZ - 10);
      const b_fr = projectPoint(bodyHalfW, -bodyHalfL, bodyElevZ - 10);
      const b_rr = projectPoint(bodyHalfW, bodyHalfL, bodyElevZ - 10);
      const b_rl = projectPoint(-bodyHalfW, bodyHalfL, bodyElevZ - 10);

      ctx.save();
      // Chassis Lower Plate
      ctx.beginPath();
      ctx.moveTo(b_fl.x, b_fl.y);
      ctx.lineTo(b_fr.x, b_fr.y);
      ctx.lineTo(b_rr.x, b_rr.y);
      ctx.lineTo(b_rl.x, b_rl.y);
      ctx.closePath();
      ctx.fillStyle = "#0a101d";
      ctx.fill();

      // Chassis Top Armor Shell
      ctx.beginPath();
      ctx.moveTo(c_fl.x, c_fl.y);
      ctx.lineTo(c_fr.x, c_fr.y);
      ctx.lineTo(c_rr.x, c_rr.y);
      ctx.lineTo(c_rl.x, c_rl.y);
      ctx.closePath();
      ctx.fillStyle = "#141e33";
      ctx.fill();
      ctx.lineWidth = 2;
      ctx.strokeStyle = "#334155";
      ctx.stroke();

      // Center Chassis ARK Logo Rendering
      const coreP = projectPoint(0, 0, bodyElevZ + 10);
      if (botLogoImg.complete && botLogoImg.naturalWidth > 0) {
        const logoSz = 34;
        ctx.shadowColor = "rgba(0, 240, 255, 0.7)";
        ctx.shadowBlur = 10;
        ctx.drawImage(botLogoImg, coreP.x - logoSz/2, coreP.y - logoSz/2, logoSz, logoSz);
        ctx.shadowBlur = 0;
      } else {
        // Fallback Reactor Core
        ctx.beginPath();
        ctx.arc(coreP.x, coreP.y, 12, 0, Math.PI * 2);
        ctx.fillStyle = "#070c18";
        ctx.fill();
        ctx.lineWidth = 2;
        ctx.strokeStyle = "var(--cyan)";
        ctx.stroke();
      }

      // Front Headlight / Lidar Indicator
      const headL = projectPoint(-18, -bodyHalfL, bodyElevZ + 10);
      const headR = projectPoint(18, -bodyHalfL, bodyElevZ + 10);
      const beamTip = projectPoint(0, -bodyHalfL - 34, bodyElevZ + 10);

      ctx.beginPath();
      ctx.moveTo(headL.x, headL.y);
      ctx.lineTo(beamTip.x, beamTip.y);
      ctx.lineTo(headR.x, headR.y);
      ctx.fillStyle = "rgba(0, 240, 255, 0.1)";
      ctx.fill();

      // Front Direction Arrow
      ctx.beginPath();
      ctx.moveTo(headL.x, headL.y);
      ctx.lineTo(headR.x, headR.y);
      ctx.strokeStyle = "#00f0ff";
      ctx.lineWidth = 3;
      ctx.stroke();

      ctx.restore();

      requestAnimationFrame(renderKinematics);
    }

    // Initialize UI on load
    buildCards();
    fetchStatus();
    setInterval(fetchStatus, 1500);
    requestAnimationFrame(renderKinematics);
  </script>
</body>
</html>
)rawliteral";

// Backwards compatibility alias
#define INDEX_HTML CALIB_HTML


// =============================================================================
// SETUP & WI-FI CONFIGURATION PAGE HTML (http://arkbot.local/setup)
// =============================================================================
const char SETUP_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no">
  <title>ARK-BOT Setup | Wi-Fi & Device Configuration</title>
  <style>
    :root {
      --bg: #070a0f;
      --card-bg: #0f1523;
      --card-border: #1e293b;
      --text-main: #f1f5f9;
      --text-dim: #94a3b8;
      --cyan: #00f0ff;
      --cyan-glow: rgba(0, 240, 255, 0.4);
      --accent: #3b82f6;
      --purple: #a855f7;
      --green: #10b981;
      --green-glow: rgba(16, 185, 129, 0.4);
      --red: #ef4444;
      --red-glow: rgba(239, 68, 68, 0.4);
      --yellow: #f59e0b;
    }
    * { box-sizing: border-box; margin: 0; padding: 0; font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, sans-serif; }
    body { background-color: var(--bg); color: var(--text-main); padding: 12px; max-width: 1000px; margin: 0 auto; padding-bottom: 50px; }
    
    /* Header */
    header { display: flex; align-items: center; justify-content: space-between; padding: 12px 18px; background: var(--card-bg); border-radius: 14px; border: 1px solid var(--card-border); margin-bottom: 14px; box-shadow: 0 8px 24px rgba(0,0,0,0.4); flex-wrap: wrap; gap: 12px; }
    .brand { display: flex; align-items: center; gap: 14px; }
    .brand-logo { width: 44px; height: 44px; object-fit: contain; filter: drop-shadow(0 0 10px var(--cyan-glow)); transition: transform 0.25s ease, filter 0.25s ease; cursor: pointer; }
    .brand-logo:hover { transform: scale(1.08) rotate(3deg); filter: drop-shadow(0 0 18px rgba(0, 240, 255, 0.9)); }
    .title h1 { font-size: 20px; font-weight: 800; letter-spacing: 0.8px; background: linear-gradient(90deg, #ffffff, var(--cyan)); -webkit-background-clip: text; -webkit-text-fill-color: transparent; }
    .title p { font-size: 11px; color: var(--text-dim); }
    
    /* Nav Tabs */
    .nav-tabs { display: flex; gap: 6px; background: rgba(0,0,0,0.4); padding: 4px; border-radius: 10px; border: 1px solid var(--card-border); }
    .nav-tab { display: inline-flex; align-items: center; gap: 6px; padding: 6px 14px; border-radius: 7px; font-size: 12px; font-weight: 700; text-decoration: none; color: var(--text-dim); transition: all 0.2s ease; }
    .nav-tab:hover { color: var(--text-main); background: rgba(255,255,255,0.05); }
    .nav-tab.active { background: linear-gradient(135deg, rgba(0,240,255,0.2), rgba(59,130,246,0.3)); color: var(--cyan); border: 1px solid var(--cyan); box-shadow: 0 0 10px var(--cyan-glow); }

    .header-right { display: flex; align-items: center; gap: 10px; flex-wrap: wrap; }
    
    /* RF Antenna Widget */
    .rf-widget { display: flex; align-items: center; gap: 6px; background: rgba(0,0,0,0.35); padding: 4px 8px; border-radius: 10px; border: 1px solid var(--card-border); }
    .rf-label { font-size: 10px; font-weight: 700; color: var(--text-dim); text-transform: uppercase; letter-spacing: 0.5px; }
    .rf-btns { display: flex; gap: 3px; }
    .rf-btn { background: #182236; border: 1px solid rgba(255,255,255,0.06); color: var(--text-dim); padding: 4px 8px; border-radius: 6px; font-size: 11px; font-weight: 700; cursor: pointer; transition: all 0.15s ease; }
    .rf-btn:hover { color: #fff; border-color: var(--cyan); }
    .rf-btn.active { background: linear-gradient(135deg, var(--cyan), var(--accent)); color: #000; font-weight: 800; border: none; box-shadow: 0 0 10px var(--cyan-glow); }
    .rssi-badge { font-size: 10px; font-weight: 700; color: #34d399; background: rgba(16, 185, 129, 0.12); padding: 2px 6px; border-radius: 4px; border: 1px solid rgba(16, 185, 129, 0.3); }

    .badge { padding: 5px 12px; border-radius: 20px; font-size: 11px; font-weight: 700; text-transform: uppercase; letter-spacing: 0.5px; }
    .badge-online { background: rgba(16, 185, 129, 0.15); color: #34d399; border: 1px solid #059669; box-shadow: 0 0 10px rgba(16, 185, 129, 0.2); }
    .badge-offline { background: rgba(239, 68, 68, 0.15); color: #f87171; border: 1px solid #dc2626; box-shadow: 0 0 10px rgba(239, 68, 68, 0.2); }

    /* Layout Grid */
    .setup-grid { display: grid; grid-template-columns: 1fr; gap: 16px; margin-top: 14px; }
    @media(min-width: 768px) { .setup-grid { grid-template-columns: 1fr 1fr; } }

    /* Cards */
    .card { background: var(--card-bg); border: 1px solid var(--card-border); border-radius: 14px; padding: 18px; box-shadow: 0 8px 24px rgba(0,0,0,0.4); position: relative; }
    .card-title { font-size: 14px; font-weight: 800; color: var(--cyan); text-transform: uppercase; letter-spacing: 0.8px; margin-bottom: 14px; display: flex; align-items: center; justify-content: space-between; }
    
    /* Wi-Fi Scan List */
    .wifi-list { max-height: 280px; overflow-y: auto; display: flex; flex-direction: column; gap: 8px; margin-top: 12px; }
    .wifi-list::-webkit-scrollbar { width: 6px; }
    .wifi-list::-webkit-scrollbar-thumb { background: #1e293b; border-radius: 3px; }
    
    .wifi-item { display: flex; align-items: center; justify-content: space-between; padding: 10px 12px; background: #131b2e; border: 1px solid rgba(255,255,255,0.05); border-radius: 10px; cursor: pointer; transition: all 0.15s ease; }
    .wifi-item:hover { border-color: var(--cyan); background: #18233c; transform: translateY(-1px); }
    .wifi-item.connected { border-color: var(--green); background: rgba(16,185,129,0.1); }
    
    .wifi-info { display: flex; align-items: center; gap: 10px; }
    .wifi-icon { font-size: 16px; }
    .wifi-ssid { font-size: 13px; font-weight: 700; color: var(--text-main); }
    .wifi-meta { font-size: 11px; color: var(--text-dim); }
    
    .wifi-badge { font-size: 10px; font-weight: 700; padding: 3px 8px; border-radius: 6px; background: #1e293b; color: var(--text-dim); border: 1px solid rgba(255,255,255,0.06); }
    .wifi-badge.conn { background: rgba(16, 185, 129, 0.2); color: #34d399; border-color: #059669; }

    /* Form Elements */
    .form-group { margin-bottom: 14px; }
    .form-label { display: block; font-size: 12px; font-weight: 700; color: var(--text-dim); text-transform: uppercase; letter-spacing: 0.5px; margin-bottom: 6px; }
    .input-wrap { position: relative; display: flex; align-items: center; }
    .input-field { width: 100%; background: #131b2e; border: 1px solid var(--card-border); color: #fff; padding: 10px 12px; border-radius: 8px; font-size: 13px; font-weight: 600; outline: none; transition: border-color 0.2s; }
    .input-field:focus { border-color: var(--cyan); box-shadow: 0 0 10px var(--cyan-glow); }
    .input-toggle { position: absolute; right: 10px; background: transparent; border: none; color: var(--text-dim); cursor: pointer; font-size: 14px; padding: 4px; }
    .input-toggle:hover { color: #fff; }

    /* Buttons */
    .btn { background: #131a29; color: var(--text-main); border: 1px solid var(--card-border); padding: 10px 16px; border-radius: 10px; font-size: 13px; font-weight: 700; cursor: pointer; transition: all 0.15s ease; display: inline-flex; align-items: center; justify-content: center; gap: 8px; user-select: none; }
    .btn:active { transform: scale(0.97); }
    .btn-green { background: linear-gradient(135deg, #059669, #10b981); border: none; color: #fff; box-shadow: 0 0 12px var(--green-glow); }
    .btn-cyan { background: linear-gradient(135deg, #00f0ff, #3b82f6); border: none; color: #000; font-weight: 800; box-shadow: 0 0 14px var(--cyan-glow); }
    .btn-red { background: linear-gradient(135deg, #b91c1c, #ef4444); border: none; color: #fff; box-shadow: 0 0 12px var(--red-glow); }
    .btn-dark { background: #182235; border: 1px solid #2d3b55; }
    .btn-dark:hover { background: #202c44; border-color: var(--cyan); }
    .btn-full { width: 100%; }

    .btn-group { display: flex; gap: 8px; flex-wrap: wrap; margin-top: 16px; }

    /* Telemetry Grid */
    .tele-grid { display: grid; grid-template-columns: 1fr 1fr; gap: 10px; }
    .tele-item { background: #131b2e; padding: 10px 12px; border-radius: 8px; border: 1px solid rgba(255,255,255,0.04); }
    .tele-label { font-size: 10px; font-weight: 700; text-transform: uppercase; color: var(--text-dim); letter-spacing: 0.5px; }
    .tele-val { font-size: 13px; font-weight: 800; color: var(--cyan); font-family: ui-monospace, SFMono-Regular, monospace; margin-top: 2px; word-break: break-all; }

    /* Spinner */
    .spinner { display: inline-block; width: 14px; height: 14px; border: 2px solid rgba(0,240,255,0.3); border-top-color: var(--cyan); border-radius: 50%; animation: spin 0.8s linear infinite; }
    @keyframes spin { to { transform: rotate(360deg); } }

    /* Modal / Overlay */
    .modal-overlay { position: fixed; inset: 0; background: rgba(0,0,0,0.8); display: none; align-items: center; justify-content: center; z-index: 200; backdrop-filter: blur(4px); }
    .modal-overlay.show { display: flex; }
    .modal-card { background: var(--card-bg); border: 1px solid var(--card-border); border-radius: 16px; padding: 24px; max-width: 400px; width: 90%; text-align: center; box-shadow: 0 16px 40px rgba(0,0,0,0.8); }
    .modal-title { font-size: 18px; font-weight: 800; color: #fff; margin-bottom: 8px; }
    .modal-body { font-size: 13px; color: var(--text-dim); margin-bottom: 20px; line-height: 1.5; }
    .modal-btns { display: flex; gap: 10px; justify-content: center; }

    /* Footer */
    .app-footer { margin-top: 36px; padding-top: 22px; border-top: 1px solid rgba(255, 255, 255, 0.08); text-align: center; }
    .footer-inner { display: flex; flex-direction: column; align-items: center; gap: 6px; }
    .footer-brand { display: flex; align-items: center; gap: 8px; font-size: 11px; font-weight: 700; color: var(--text-dim); letter-spacing: 1.2px; text-transform: uppercase; }
    .footer-dot { width: 6px; height: 6px; border-radius: 50%; background: var(--cyan); box-shadow: 0 0 8px var(--cyan-glow); }
    .footer-credit { font-size: 13px; color: #cbd5e1; font-weight: 500; }
    .author-name { font-weight: 800; color: #ffffff; background: linear-gradient(90deg, #ffffff, var(--cyan)); -webkit-background-clip: text; -webkit-text-fill-color: transparent; }
    .footer-org { font-size: 11px; color: var(--text-dim); letter-spacing: 0.5px; }

    /* Toast Notification */
    #toast { position: fixed; bottom: 24px; left: 50%; transform: translateX(-50%); background: #10b981; color: white; padding: 10px 22px; border-radius: 30px; font-size: 13px; font-weight: 700; box-shadow: 0 8px 24px rgba(0,0,0,0.6); opacity: 0; pointer-events: none; transition: opacity 0.2s ease; z-index: 100; border: 1px solid rgba(255,255,255,0.2); }
    #toast.show { opacity: 1; }
  </style>
</head>
<body>

  <header>
    <div class="brand">
      <img src=")rawliteral" ARK_LOGO_SRC R"rawliteral(" class="brand-logo" alt="ARK-BOT Logo" onclick="location.href='/calib'">
      <div class="title">
        <h1>ARK-BOT</h1>
        <p>System Setup & Wi-Fi Configuration</p>
      </div>
    </div>

    <!-- Navigation Tabs -->
    <nav class="nav-tabs">
      <a href="/calib" class="nav-tab">🎯 Calibrator</a>
      <a href="/setup" class="nav-tab active">⚙️ Setup</a>
    </nav>
    
    <div class="header-right">
      <!-- Antenna Control -->
      <div class="rf-widget">
        <span class="rf-label">Antenna:</span>
        <div class="rf-btns">
          <button class="rf-btn active" id="btnAntInt" onclick="setAntenna('internal')" title="Switch to Onboard Ceramic Antenna">📡 Int</button>
          <button class="rf-btn" id="btnAntExt" onclick="setAntenna('external')" title="Switch to External IPEX/U.FL Antenna">🛰️ Ext</button>
        </div>
        <span class="rssi-badge" id="rssiVal">📶 -- dBm</span>
      </div>
      
      <div id="wifiBadge" class="badge badge-online">WIFI: CHECKING</div>
    </div>
  </header>

  <div class="setup-grid">
    <!-- Card 1: Wi-Fi Scanner -->
    <div class="card">
      <div class="card-title">
        <span>📡 Wi-Fi Scanner</span>
        <button class="btn btn-dark" id="btnScan" onclick="triggerScan()">
          <span id="scanIcon">🔍</span> <span id="scanText">Scan Networks</span>
        </button>
      </div>
      <p style="font-size:12px; color:var(--text-dim);">Discovered 2.4GHz Wi-Fi networks in range. Click any network to auto-fill credentials.</p>
      
      <div class="wifi-list" id="wifiList">
        <div style="text-align:center; padding:24px; color:var(--text-dim); font-size:12px;">
          Click <strong>Scan Networks</strong> to search for nearby Wi-Fi access points.
        </div>
      </div>
    </div>

    <!-- Card 2: Wi-Fi Credentials & Configuration -->
    <div class="card">
      <div class="card-title">
        <span>🔒 Wi-Fi Credentials</span>
      </div>
      <form id="wifiForm" onsubmit="event.preventDefault(); saveWifi();">
        <div class="form-group">
          <label class="form-label" for="ssidInput">Network SSID (Name)</label>
          <div class="input-wrap">
            <input type="text" id="ssidInput" class="input-field" placeholder="Enter or select Wi-Fi SSID" required autocomplete="off">
          </div>
        </div>

        <div class="form-group">
          <label class="form-label" for="passInput">Wi-Fi Password</label>
          <div class="input-wrap">
            <input type="password" id="passInput" class="input-field" placeholder="Enter Wi-Fi Password" autocomplete="current-password">
            <button type="button" class="input-toggle" onclick="togglePassVisibility()" title="Toggle password visibility">👁️</button>
          </div>
        </div>

        <div class="btn-group">
          <button type="submit" class="btn btn-green btn-full" id="btnSave">
            ⚡ Save Credentials & Connect
          </button>
          <button type="button" class="btn btn-dark" style="flex:1;" onclick="reconnectWifi()">
            🔄 Reconnect
          </button>
          <button type="button" class="btn btn-dark" style="flex:1;" onclick="resetWifi()">
            🗑️ Clear Saved
          </button>
        </div>
      </form>
    </div>

    <!-- Card 3: Network & System Telemetry -->
    <div class="card">
      <div class="card-title">
        <span>📊 Connection & System Status</span>
      </div>
      <div class="tele-grid">
        <div class="tele-item">
          <div class="tele-label">Wi-Fi Mode</div>
          <div class="tele-val" id="teleMode">--</div>
        </div>
        <div class="tele-item">
          <div class="tele-label">Station IP</div>
          <div class="tele-val" id="teleStaIp">--</div>
        </div>
        <div class="tele-item">
          <div class="tele-label">SoftAP SSID / IP</div>
          <div class="tele-val" id="teleAp">ARK-BOT-AP<br>192.168.4.1</div>
        </div>
        <div class="tele-item">
          <div class="tele-label">mDNS Hostname</div>
          <div class="tele-val" id="teleMdns">arkbot.local</div>
        </div>
        <div class="tele-item">
          <div class="tele-label">Device MAC</div>
          <div class="tele-val" id="teleMac">--</div>
        </div>
        <div class="tele-item">
          <div class="tele-label">Signal RSSI</div>
          <div class="tele-val" id="teleRssi">-- dBm</div>
        </div>
        <div class="tele-item">
          <div class="tele-label">RF Antenna</div>
          <div class="tele-val" id="teleAnt">Internal Ceramic</div>
        </div>
        <div class="tele-item">
          <div class="tele-label">System Uptime</div>
          <div class="tele-val" id="teleUptime">0s</div>
        </div>
      </div>
    </div>

    <!-- Card 4: Device Management -->
    <div class="card">
      <div class="card-title">
        <span>⚡ Hardware & Power Management</span>
      </div>
      <p style="font-size:12px; color:var(--text-dim); margin-bottom:14px; line-height:1.5;">
        Perform system maintenance or restart the Seeed Studio XIAO ESP32-C6 controller board.
      </p>

      <div class="btn-group">
        <button class="btn btn-red btn-full" onclick="confirmReboot()">
          🔄 Restart Robot Controller
        </button>
      </div>

      <div style="margin-top:16px; padding:12px; background:#131b2e; border-radius:8px; border:1px solid rgba(255,255,255,0.04);">
        <div style="font-size:11px; font-weight:700; color:var(--cyan); margin-bottom:4px;">💡 SoftAP Fallback Info</div>
        <div style="font-size:11px; color:var(--text-dim); line-height:1.4;">
          If the configured home network is unreachable, ARK-BOT automatically broadcasts its fallback access point <strong>ARK-BOT-AP</strong> (password: <code>12345678</code>) at <code>192.168.4.1</code>.
        </div>
      </div>
    </div>
  </div>

  <!-- Reboot Confirmation Modal -->
  <div class="modal-overlay" id="rebootModal">
    <div class="modal-card">
      <div class="modal-title">Restart Robot Controller?</div>
      <div class="modal-body">
        The ESP32-C6 will restart and re-initialize the PCA9685 servo driver, OLED display, and Wi-Fi stack.
      </div>
      <div class="modal-btns">
        <button class="btn btn-dark" onclick="closeRebootModal()">Cancel</button>
        <button class="btn btn-red" onclick="executeReboot()">Yes, Restart Now</button>
      </div>
    </div>
  </div>

  <!-- App Footer -->
  <footer class="app-footer">
    <div class="footer-inner">
      <div class="footer-brand">
        <span class="footer-dot"></span>
        <span class="footer-title">ARK-BOT SYSTEM &bull; v0.1.2</span>
      </div>
      <div class="footer-credit">
        Designed & Engineered by <span class="author-name">Amuthesan</span>
      </div>
      <div class="footer-org">Ark Technology</div>
    </div>
  </footer>

  <div id="toast">Command Sent</div>

  <script>
    let isExternalAntenna = false;

    function togglePassVisibility() {
      const pass = document.getElementById('passInput');
      pass.type = (pass.type === 'password') ? 'text' : 'password';
    }

    async function triggerScan() {
      const btn = document.getElementById('btnScan');
      const icon = document.getElementById('scanIcon');
      const text = document.getElementById('scanText');
      const list = document.getElementById('wifiList');

      btn.disabled = true;
      icon.innerHTML = '<span class="spinner"></span>';
      text.innerText = 'Scanning...';
      list.innerHTML = '<div style="text-align:center; padding:24px; color:var(--cyan);"><span class="spinner" style="margin-right:8px;"></span> Scanning 2.4GHz Wi-Fi channels...</div>';

      try {
        const res = await fetch('/api/wifi/scan');
        const networks = await res.json();
        renderWifiList(networks);
        showToast(`Discovered ${networks.length} Wi-Fi networks`);
      } catch (err) {
        console.error("Wi-Fi scan failed", err);
        list.innerHTML = '<div style="text-align:center; padding:24px; color:var(--red);">Scan failed. Please try again.</div>';
      } finally {
        btn.disabled = false;
        icon.innerText = '🔍';
        text.innerText = 'Scan Networks';
      }
    }

    function renderWifiList(networks) {
      const list = document.getElementById('wifiList');
      if (!networks || networks.length === 0) {
        list.innerHTML = '<div style="text-align:center; padding:24px; color:var(--text-dim); font-size:12px;">No networks found in range.</div>';
        return;
      }

      // Sort by RSSI descending (strongest first)
      networks.sort((a, b) => b.rssi - a.rssi);

      let html = '';
      networks.forEach(net => {
        const isConn = net.connected;
        let signalIcon = '📶';
        let signalClass = '#10b981';
        if (net.rssi < -80) signalClass = '#ef4444';
        else if (net.rssi < -68) signalClass = '#f59e0b';

        const authText = net.auth || 'Open';
        const lockIcon = (authText.toLowerCase() === 'open') ? '🔓' : '🔒';

        html += `
          <div class="wifi-item ${isConn ? 'connected' : ''}" onclick="selectNetwork('${escapeHtml(net.ssid)}')">
            <div class="wifi-info">
              <span class="wifi-icon" style="color:${signalClass}">${signalIcon}</span>
              <div>
                <div class="wifi-ssid">${escapeHtml(net.ssid)}</div>
                <div class="wifi-meta">${net.rssi} dBm &bull; Ch ${net.channel} &bull; ${lockIcon} ${authText}</div>
              </div>
            </div>
            <div>
              ${isConn ? '<span class="wifi-badge conn">CONNECTED</span>' : '<span class="wifi-badge">SELECT</span>'}
            </div>
          </div>
        `;
      });
      list.innerHTML = html;
    }

    function selectNetwork(ssid) {
      document.getElementById('ssidInput').value = ssid;
      const pass = document.getElementById('passInput');
      pass.focus();
      showToast(`Selected "${ssid}". Enter password if required.`);
    }

    function escapeHtml(str) {
      return (str || '').replace(/[&<>"']/g, m => ({
        '&': '&amp;', '<': '&lt;', '>': '&gt;', '"': '&quot;', "'": '&#39;'
      }[m]));
    }

    async function saveWifi() {
      const ssid = document.getElementById('ssidInput').value.trim();
      const pass = document.getElementById('passInput').value;

      if (!ssid) {
        showToast("Please enter a valid SSID");
        return;
      }

      showToast("Saving credentials and connecting...");
      const btn = document.getElementById('btnSave');
      btn.disabled = true;
      btn.innerText = "Saving & Connecting...";

      try {
        const res = await fetch('/api/wifi/save', {
          method: 'POST',
          headers: { 'Content-Type': 'application/x-www-form-urlencoded' },
          body: `ssid=${encodeURIComponent(ssid)}&pass=${encodeURIComponent(pass)}`
        });
        const data = await res.json();
        showToast(data.message || "Wi-Fi credentials saved!");
        setTimeout(fetchWifiStatus, 3000);
      } catch (err) {
        console.error("Save Wi-Fi error", err);
        showToast("Error saving Wi-Fi settings");
      } finally {
        btn.disabled = false;
        btn.innerText = "⚡ Save Credentials & Connect";
      }
    }

    async function reconnectWifi() {
      showToast("Triggering Wi-Fi reconnection...");
      try {
        await fetch('/api/wifi/reconnect', { method: 'POST' });
        setTimeout(fetchWifiStatus, 3000);
      } catch (err) {
        console.error("Reconnect error", err);
      }
    }

    async function resetWifi() {
      if (!confirm("Are you sure you want to clear saved Wi-Fi credentials? ARK-BOT will fall back to default configuration.")) return;
      showToast("Resetting Wi-Fi settings...");
      try {
        await fetch('/api/wifi/reset', { method: 'POST' });
        showToast("Wi-Fi settings reset to default");
        fetchWifiStatus();
      } catch (err) {
        console.error("Reset Wi-Fi error", err);
      }
    }

    async function setAntenna(type) {
      showToast(`Switching to ${type.toUpperCase()} Antenna...`);
      try {
        const res = await fetch(`/api/antenna?type=${type}`, { method: 'POST' });
        const data = await res.json();
        updateAntennaUI(data.extAntenna, data.rssi);
        showToast(`Antenna switched: ${data.extAntenna ? 'EXTERNAL (IPEX)' : 'INTERNAL (Ceramic)'}`);
      } catch (err) {
        console.error("Antenna API error", err);
      }
    }

    function updateAntennaUI(isExt, rssi) {
      isExternalAntenna = isExt;
      const btnInt = document.getElementById('btnAntInt');
      const btnExt = document.getElementById('btnAntExt');
      const rssiBadge = document.getElementById('rssiVal');
      const teleAnt = document.getElementById('teleAnt');
      
      if (btnInt && btnExt) {
        btnInt.className = isExt ? 'rf-btn' : 'rf-btn active';
        btnExt.className = isExt ? 'rf-btn active' : 'rf-btn';
      }
      if (rssiBadge && rssi !== undefined) {
        rssiBadge.innerText = `📶 ${rssi} dBm`;
      }
      if (teleAnt) {
        teleAnt.innerText = isExt ? 'External (IPEX)' : 'Internal Ceramic';
      }
    }

    function confirmReboot() {
      document.getElementById('rebootModal').classList.add('show');
    }

    function closeRebootModal() {
      document.getElementById('rebootModal').classList.remove('show');
    }

    async function executeReboot() {
      closeRebootModal();
      showToast("Sending Reboot command... ESP32 restarting...");
      try {
        await fetch('/api/reboot', { method: 'POST' });
      } catch (e) {}

      let countdown = 8;
      const toast = document.getElementById('toast');
      const timer = setInterval(() => {
        countdown--;
        if (countdown > 0) {
          toast.innerText = `Rebooting... Reconnecting in ${countdown}s`;
          toast.classList.add('show');
        } else {
          clearInterval(timer);
          location.reload();
        }
      }, 1000);
    }

    async function fetchWifiStatus() {
      try {
        const res = await fetch('/api/wifi/status');
        const data = await res.json();
        
        const badge = document.getElementById('wifiBadge');
        if (badge) {
          if (data.connected) {
            badge.className = 'badge badge-online';
            badge.innerText = `STA: ${data.staSsid || 'ONLINE'}`;
          } else {
            badge.className = 'badge badge-offline';
            badge.innerText = 'MODE: SoftAP ONLY';
          }
        }

        document.getElementById('teleMode').innerText = data.connected ? `STA (${data.staSsid})` : 'SoftAP Fallback';
        document.getElementById('teleStaIp').innerText = data.staIp || 'Not Connected';
        document.getElementById('teleAp').innerHTML = `${data.apSsid}<br>${data.apIp}`;
        document.getElementById('teleMdns').innerText = data.hostname || 'arkbot.local';
        document.getElementById('teleMac').innerText = data.mac || '--';
        document.getElementById('teleRssi').innerText = `${data.rssi || 0} dBm`;

        if (data.extAntenna !== undefined) {
          updateAntennaUI(data.extAntenna, data.rssi);
        }

        if (data.uptime !== undefined) {
          const s = data.uptime;
          const m = Math.floor(s / 60) % 60;
          const h = Math.floor(s / 3600);
          document.getElementById('teleUptime').innerText = `${h}h ${m}m ${s % 60}s`;
        }

        // Auto fill SSID input if empty
        const ssidInput = document.getElementById('ssidInput');
        if (!ssidInput.value && data.staSsid) {
          ssidInput.value = data.staSsid;
        }
      } catch (err) {
        console.error("Fetch Wi-Fi status failed", err);
      }
    }

    function showToast(msg) {
      const toast = document.getElementById('toast');
      toast.innerText = msg;
      toast.classList.add('show');
      setTimeout(() => toast.classList.remove('show'), 2500);
    }

    // Initialize on load
    fetchWifiStatus();
    setInterval(fetchWifiStatus, 3000);
    setTimeout(triggerScan, 400);
  </script>
</body>
</html>
)rawliteral";
