#pragma once

#include <Arduino.h>

const char INDEX_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no">
  <title>ARK-BOT Cyber Kinematics Controller</title>
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
      <img src="data:image/png;base64,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" class="brand-logo" alt="ARK-BOT Logo">
      <div class="title">
        <h1>ARK-BOT</h1>
        <p>Cyber Kinematics Engine (ESP32-C6 + PCA9685)</p>
      </div>
    </div>
    
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
        <span class="footer-title">ARK-BOT SYSTEM &bull; v0.1.1</span>
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
    botLogoImg.src = "data:image/png;base64,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";

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
      document.getElementById('activeCount').innerText = `${activeCount} / 12 Servos Active`;
    }

    async function sendServoAngle(leg, joint, angle) {
      try {
        await fetch(`/api/servo?leg=${leg}&joint=${joint}&angle=${angle}`, { method: 'POST' });
      } catch (err) {
        console.error("Servo API error", err);
      }
    }

    async function sendInit(type) {
      showToast(type === 'wave' ? "Running Sequential Wave Calibration..." : "Centering All Servos to 90°...");
      try {
        const res = await fetch(`/api/init?type=${type}`, { method: 'POST' });
        const data = await res.json();
        if (data.angles) {
          updateUIWithState(data.angles, data.enabled);
        }
      } catch (err) {
        console.error("Init API error", err);
      }
    }

    async function sendBeep() {
      showToast("Beeping Buzzer...");
      try {
        await fetch('/api/beep', { method: 'POST' });
      } catch (err) {
        console.error("Beep API error", err);
      }
    }

    function updateUIWithState(angles, enabled) {
      for (let l = 0; l < 4; l++) {
        for (let j = 0; j < 3; j++) {
          const val = angles[l][j];
          targetAngles[l][j] = val;
          const slider = document.getElementById(`slider_${l}_${j}`);
          const label = document.getElementById(`val_${l}_${j}`);
          if (slider && document.activeElement !== slider) slider.value = val;
          if (label) label.innerText = `${val}°`;
          
          if (enabled) {
            updateSinglePowerUI(l, j, enabled[l][j]);
          }
        }
        updateLegHeaderPowerUI(l);
      }
    }

    async function fetchStatus() {
      try {
        const res = await fetch('/api/status');
        const data = await res.json();
        
        const badge = document.getElementById('pcaStatus');
        if (data.pcaReady) {
          badge.className = "badge badge-online";
          badge.innerText = "PCA: ONLINE (0x40)";
        } else {
          badge.className = "badge badge-offline";
          badge.innerText = "PCA: MISSING";
        }

        if (data.extAntenna !== undefined) {
          updateAntennaUI(data.extAntenna, data.rssi);
        }

        if (data.angles && !throttleTimer) {
          updateUIWithState(data.angles, data.enabled);
        }
      } catch (err) {
        const badge = document.getElementById('pcaStatus');
        badge.className = "badge badge-offline";
        badge.innerText = "DISCONNECTED";
      }
    }

    function showToast(msg) {
      const toast = document.getElementById('toast');
      toast.innerText = msg;
      toast.classList.add('show');
      setTimeout(() => toast.classList.remove('show'), 1600);
    }

    // ==========================================
    // Accurate Physical Kinematics Canvas Engine
    // ==========================================
    const canvas = document.getElementById('robotCanvas');
    const ctx = canvas.getContext('2d');

    function resizeCanvas() {
      const rect = canvas.parentElement.getBoundingClientRect();
      const dpr = window.devicePixelRatio || 1;
      canvas.width = rect.width * dpr;
      canvas.height = rect.height * dpr;
      ctx.scale(dpr, dpr);
    }
    window.addEventListener('resize', resizeCanvas);
    resizeCanvas();

    function renderKinematics() {
      const w = canvas.width / (window.devicePixelRatio || 1);
      const h = canvas.height / (window.devicePixelRatio || 1);
      ctx.clearRect(0, 0, w, h);

      // Physics LERP interpolation for smooth mechanical motion
      for (let l = 0; l < 4; l++) {
        for (let j = 0; j < 3; j++) {
          currentSimAngles[l][j] += (targetAngles[l][j] - currentSimAngles[l][j]) * 0.16;
        }
      }

      const cx = w / 2;
      const cy = h / 2;
      waveAnimPhase += 0.035;

      // 1. Draw Tech Background Radar Grid
      ctx.save();
      ctx.strokeStyle = "rgba(30, 41, 59, 0.4)";
      ctx.lineWidth = 1;
      for (let x = 0; x < w; x += 36) {
        ctx.beginPath();
        ctx.moveTo(x, 0);
        ctx.lineTo(x, h);
        ctx.stroke();
      }
      for (let y = 0; y < h; y += 36) {
        ctx.beginPath();
        ctx.moveTo(0, y);
        ctx.lineTo(w, y);
        ctx.stroke();
      }

      // Range Target Rings
      ctx.strokeStyle = "rgba(0, 240, 255, 0.08)";
      [75, 140, 200].forEach(r => {
        ctx.beginPath();
        ctx.arc(cx, cy, r, 0, Math.PI * 2);
        ctx.stroke();
      });

      // HUD Telemetry Overlay
      ctx.font = "bold 9px monospace";
      ctx.fillStyle = "rgba(0, 240, 255, 0.6)";
      ctx.fillText(`VIEW: ${viewMode.toUpperCase()}`, 14, 18);
      ctx.fillText(`CALIBRATION: 90° NEUTRAL`, 14, 30);
      ctx.fillText(`ANT: ${isExternalAntenna ? 'EXT (IPEX)' : 'INT (CERAMIC)'}`, w - 145, 18);
      ctx.restore();

      // Physical Robot Dimensions (in simulated mm / units)
      const bodyHalfW = 42;   // Half chassis width (Left to Right)
      const bodyHalfL = 68;   // Half chassis length (Front to Rear)
      const bodyElevZ = 52;   // Chassis standing height above ground

      const coxaLength = 34;   // Coxa horn extends straight out horizontally
      const femurLength = 48;  // Thigh length
      const tibiaLength = 52;  // Shin length

      // 4 Hip Base Mounts on Body Corners:
      const legMounts = [
        { name: "FR", x: bodyHalfW,  y: -bodyHalfL, sideX: 1,  frontY: -1 },
        { name: "FL", x: -bodyHalfW, y: -bodyHalfL, sideX: -1, frontY: -1 },
        { name: "RR", x: bodyHalfW,  y: bodyHalfL,  sideX: 1,  frontY: 1 },
        { name: "RL", x: -bodyHalfW, y: bodyHalfL,  sideX: -1, frontY: 1 }
      ];

      // Perspective Projection Function
      function projectPoint(x, y, z) {
        if (viewMode === 'top') {
          return { x: cx + x * 1.15, y: cy + y * 1.15 };
        } else if (viewMode === 'side') {
          return { x: cx + y * 1.15, y: cy - (z - bodyElevZ) * 1.15 };
        } else {
          const isoX = (x - y * 0.5) * 1.0;
          const isoY = (x * 0.28 + y * 0.58) - (z - bodyElevZ) * 0.95;
          return { x: cx + isoX, y: cy + isoY };
        }
      }

      // Draw Ground Shadows First
      legMounts.forEach((m, l) => {
        const angles = currentSimAngles[l];
        const deltaYaw = ((angles[0] - 90) * Math.PI) / 180;
        const deltaFemur = ((angles[1] - 90) * Math.PI) / 180;
        const deltaTibia = ((angles[2] - 90) * Math.PI) / 180;

        const hipX = m.x;
        const hipY = m.y;
        const hipZ = bodyElevZ;

        const coxaYaw = (m.sideX > 0 ? 0 : Math.PI) + deltaYaw * m.sideX;
        const coxaEndX = hipX + Math.cos(coxaYaw) * coxaLength;
        const coxaEndY = hipY + Math.sin(coxaYaw) * coxaLength;
        const coxaEndZ = hipZ;

        const femurPitch = -Math.PI / 4 + deltaFemur;
        const femurSpan = Math.cos(femurPitch) * femurLength;
        const kneeX = coxaEndX + Math.cos(coxaYaw) * femurSpan;
        const kneeY = coxaEndY + Math.sin(coxaYaw) * femurSpan;
        const kneeZ = coxaEndZ + Math.sin(femurPitch) * femurLength;

        const tibiaPitch = femurPitch - Math.PI / 2 + deltaTibia;
        const tibiaSpan = Math.cos(tibiaPitch) * tibiaLength;
        const footX = kneeX + Math.cos(coxaYaw) * tibiaSpan;
        const footY = kneeY + Math.sin(coxaYaw) * tibiaSpan;

        const g_hip = projectPoint(hipX, hipY, 0);
        const g_coxa = projectPoint(coxaEndX, coxaEndY, 0);
        const g_knee = projectPoint(kneeX, kneeY, 0);
        const g_foot = projectPoint(footX, footY, 0);

        ctx.save();
        ctx.beginPath();
        ctx.moveTo(g_hip.x, g_hip.y);
        ctx.lineTo(g_coxa.x, g_coxa.y);
        ctx.lineTo(g_knee.x, g_knee.y);
        ctx.lineTo(g_foot.x, g_foot.y);
        ctx.strokeStyle = "rgba(0, 0, 0, 0.3)";
        ctx.lineWidth = 4;
        ctx.stroke();
        ctx.restore();
      });

      // Draw Limbs & Joints
      legMounts.forEach((m, l) => {
        const angles = currentSimAngles[l];
        const pwr = servoPower[l];

        const deltaYaw = ((angles[0] - 90) * Math.PI) / 180;
        const deltaFemur = ((angles[1] - 90) * Math.PI) / 180;
        const deltaTibia = ((angles[2] - 90) * Math.PI) / 180;

        const hipX = m.x;
        const hipY = m.y;
        const hipZ = bodyElevZ;

        const coxaYaw = (m.sideX > 0 ? 0 : Math.PI) + deltaYaw * m.sideX;
        const coxaEndX = hipX + Math.cos(coxaYaw) * coxaLength;
        const coxaEndY = hipY + Math.sin(coxaYaw) * coxaLength;
        const coxaEndZ = hipZ;

        const femurPitch = -Math.PI / 4 + deltaFemur;
        const femurSpan = Math.cos(femurPitch) * femurLength;
        const kneeX = coxaEndX + Math.cos(coxaYaw) * femurSpan;
        const kneeY = coxaEndY + Math.sin(coxaYaw) * femurSpan;
        const kneeZ = coxaEndZ + Math.sin(femurPitch) * femurLength;

        const tibiaPitch = femurPitch - Math.PI / 2 + deltaTibia;
        const tibiaSpan = Math.cos(tibiaPitch) * tibiaLength;
        const footX = kneeX + Math.cos(coxaYaw) * tibiaSpan;
        const footY = kneeY + Math.sin(coxaYaw) * tibiaSpan;
        const footZ = kneeZ + Math.sin(tibiaPitch) * tibiaLength;

        const p0 = projectPoint(hipX, hipY, hipZ);
        const p1 = projectPoint(coxaEndX, coxaEndY, coxaEndZ);
        const p2 = projectPoint(kneeX, kneeY, kneeZ);
        const p3 = projectPoint(footX, footY, footZ);

        const isCoxaOn = pwr[0];
        const isFemurOn = pwr[1];
        const isTibiaOn = pwr[2];

        // 1. Coxa Segment (Shoulder Mount)
        ctx.beginPath();
        ctx.moveTo(p0.x, p0.y);
        ctx.lineTo(p1.x, p1.y);
        ctx.strokeStyle = isCoxaOn ? "#00f0ff" : "#ef4444";
        ctx.lineWidth = 6;
        ctx.lineCap = "round";
        if (!isCoxaOn) ctx.setLineDash([4, 4]);
        ctx.stroke();

        // 2. Femur Segment (Thigh Strut)
        ctx.beginPath();
        ctx.moveTo(p1.x, p1.y);
        ctx.lineTo(p2.x, p2.y);
        ctx.strokeStyle = isFemurOn ? "#3b82f6" : "#ef4444";
        ctx.lineWidth = 5;
        if (!isFemurOn) ctx.setLineDash([4, 4]);
        ctx.stroke();

        // 3. Tibia Segment (Shin Piston)
        ctx.beginPath();
        ctx.moveTo(p2.x, p2.y);
        ctx.lineTo(p3.x, p3.y);
        ctx.strokeStyle = isTibiaOn ? "#00f0ff" : "#ef4444";
        ctx.lineWidth = 4;
        if (!isTibiaOn) ctx.setLineDash([4, 4]);
        ctx.stroke();
        ctx.setLineDash([]);

        // Joint Node Bearings
        ctx.beginPath();
        ctx.arc(p0.x, p0.y, 6, 0, Math.PI * 2);
        ctx.fillStyle = isCoxaOn ? "#00f0ff" : "#475569";
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
