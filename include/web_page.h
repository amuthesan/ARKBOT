#pragma once

#include <Arduino.h>

// Shared Brand Logo Data URL (High-definition Cyber Shield Quadruped SVG)
#define ARK_LOGO_SRC "data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' viewBox='0 0 120 120' width='120' height='120'><defs><linearGradient id='cg' x1='0%25' y1='0%25' x2='100%25' y2='100%25'><stop offset='0%25' stop-color='%2300f0ff'/><stop offset='100%25' stop-color='%233b82f6'/></linearGradient><linearGradient id='bg' x1='0%25' y1='0%25' x2='100%25' y2='100%25'><stop offset='0%25' stop-color='%231e293b'/><stop offset='100%25' stop-color='%23090d16'/></linearGradient><filter id='gl' x='-20%25' y='-20%25' width='140%25' height='140%25'><feGaussianBlur stdDeviation='3' result='b'/><feComposite in='SourceGraphic' in2='b' operator='over'/></filter></defs><polygon points='60,6 108,33 108,87 60,114 12,87 12,33' fill='url(%23bg)' stroke='url(%23cg)' stroke-width='4' filter='url(%23gl)'/><polygon points='60,18 96,39 96,81 60,102 24,81 24,39' fill='none' stroke='%2300f0ff' stroke-width='1.5' stroke-dasharray='4,3' opacity='0.6'/><rect x='44' y='46' width='32' height='28' rx='6' fill='%230f172a' stroke='%2300f0ff' stroke-width='2'/><path d='M44,48 L30,36 L18,52' fill='none' stroke='%2300f0ff' stroke-width='3' stroke-linecap='round' stroke-linejoin='round'/><path d='M76,48 L90,36 L102,52' fill='none' stroke='%2300f0ff' stroke-width='3' stroke-linecap='round' stroke-linejoin='round'/><path d='M44,70 L30,84 L18,74' fill='none' stroke='%233b82f6' stroke-width='3' stroke-linecap='round' stroke-linejoin='round'/><path d='M76,70 L90,84 L102,74' fill='none' stroke='%233b82f6' stroke-width='3' stroke-linecap='round' stroke-linejoin='round'/><circle cx='60' cy='60' r='7' fill='url(%23cg)' filter='url(%23gl)'/><circle cx='60' cy='60' r='3' fill='%23ffffff'/><polygon points='52,38 68,38 64,44 56,44' fill='%2300f0ff' filter='url(%23gl)'/><path d='M50,92 L60,78 L70,92 M53,88 L67,88' fill='none' stroke='%2300f0ff' stroke-width='2' stroke-linecap='round' opacity='0.9'/></svg>"

// =============================================================================
// 1. COMMANDER / MAIN HOMEPAGE HTML (http://arkbot.local/)
// =============================================================================
const char COMMANDER_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no">
  <title>ARK-BOT Action Commander | v1.0.6</title>
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
    
    /* RF Antenna & Telemetry Widgets */
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
    .badge-busy { background: rgba(245, 158, 11, 0.2); color: #fbbf24; border: 1px solid #d97706; box-shadow: 0 0 10px rgba(245, 158, 11, 0.3); }

    /* Visual Simulation & IMU Grid */
    .tele-sim-grid { display: grid; grid-template-columns: 1fr; gap: 14px; margin-bottom: 16px; }
    @media(min-width: 840px) { .tele-sim-grid { grid-template-columns: 1.65fr 1fr; } }

    .sim-card { background: radial-gradient(circle at center, #131b2e 0%, #0a0e18 100%); border: 1px solid var(--card-border); border-radius: 14px; padding: 14px; position: relative; overflow: hidden; box-shadow: 0 12px 32px rgba(0,0,0,0.5); display: flex; flex-direction: column; justify-content: space-between; }
    .sim-topbar { display: flex; justify-content: space-between; align-items: center; margin-bottom: 10px; flex-wrap: wrap; gap: 8px; }
    .sim-title { font-size: 12px; font-weight: 700; text-transform: uppercase; color: var(--cyan); letter-spacing: 1px; display: flex; align-items: center; gap: 8px; }
    .view-toggles { display: flex; gap: 6px; background: rgba(0,0,0,0.3); padding: 3px; border-radius: 8px; border: 1px solid var(--card-border); }
    .view-btn { background: transparent; border: none; color: var(--text-dim); padding: 4px 10px; border-radius: 6px; font-size: 11px; font-weight: 700; cursor: pointer; transition: all 0.15s ease; }
    .view-btn.active { background: var(--accent); color: #fff; box-shadow: 0 0 8px var(--cyan-glow); }
    
    .canvas-container { position: relative; width: 100%; height: 320px; border-radius: 10px; background: #070a12; border: 1px solid rgba(255,255,255,0.05); overflow: hidden; }
    canvas { width: 100%; height: 100%; display: block; }
    
    .sim-footer { display: flex; justify-content: space-between; align-items: center; margin-top: 10px; font-size: 11px; color: var(--text-dim); flex-wrap: wrap; gap: 8px; }
    .sim-status { font-weight: 700; color: var(--cyan); }

    /* Artificial Horizon Instrument Panel */
    .horizon-panel { align-items: center; text-align: center; }
    .horizon-canvas-wrap { width: 180px; height: 180px; margin: 10px auto; position: relative; border-radius: 50%; box-shadow: 0 0 24px rgba(0, 240, 255, 0.2); }
    .horizon-canvas-wrap canvas { width: 180px; height: 180px; border-radius: 50%; }
    .horizon-metrics { display: flex; justify-content: space-between; width: 100%; margin-top: 8px; gap: 8px; }
    .h-metric-item { background: rgba(0,0,0,0.35); border: 1px solid var(--card-border); padding: 8px 12px; border-radius: 10px; flex: 1; }
    .h-metric-label { font-size: 10px; font-weight: 700; color: var(--text-dim); display: block; margin-bottom: 2px; }
    .h-metric-val { font-size: 18px; font-weight: 800; color: var(--cyan); font-family: ui-monospace, SFMono-Regular, monospace; }
    .h-accel-row { font-size: 11px; color: var(--text-dim); margin-top: 8px; font-family: ui-monospace, SFMono-Regular, monospace; background: rgba(0,0,0,0.25); padding: 6px 10px; border-radius: 8px; border: 1px solid rgba(255,255,255,0.04); width: 100%; }
    .h-accel-row b { color: #f1f5f9; font-weight: 700; }

    /* Commander Controls Grid */
    .commander-grid { display: grid; grid-template-columns: 1fr; gap: 16px; margin-bottom: 18px; }
    @media(min-width: 768px) { .commander-grid { grid-template-columns: 1fr 1fr; } }

    .card { background: var(--card-bg); border: 1px solid var(--card-border); border-radius: 14px; padding: 18px; box-shadow: 0 8px 24px rgba(0,0,0,0.4); }
    .card-title { font-size: 13px; font-weight: 800; color: var(--cyan); text-transform: uppercase; letter-spacing: 0.8px; margin-bottom: 14px; display: flex; align-items: center; justify-content: space-between; }

    /* D-PAD Motion Controller */
    .dpad-container { display: flex; flex-direction: column; align-items: center; justify-content: center; gap: 8px; padding: 10px 0; }
    .dpad-row { display: flex; gap: 8px; justify-content: center; width: 100%; }

    .btn-action { width: 90px; height: 70px; background: #131b2c; border: 1px solid var(--card-border); color: var(--text-main); border-radius: 12px; display: flex; flex-direction: column; align-items: center; justify-content: center; gap: 4px; font-size: 11px; font-weight: 800; cursor: pointer; transition: all 0.15s ease; user-select: none; }
    .btn-action span.icon { font-size: 22px; }
    .btn-action:hover { border-color: var(--cyan); transform: translateY(-2px); box-shadow: 0 4px 14px var(--cyan-glow); }
    .btn-action:active { transform: scale(0.95); }

    .btn-action.fwd { background: linear-gradient(135deg, rgba(0,240,255,0.15), #182848); color: var(--cyan); border-color: rgba(0,240,255,0.4); }
    .btn-action.stop { background: linear-gradient(135deg, rgba(239,68,68,0.2), #2a1118); color: #f87171; border-color: rgba(239,68,68,0.5); }
    .btn-action.turn { background: linear-gradient(135deg, rgba(59,130,246,0.15), #18223c); color: #60a5fa; }
    .btn-action.back { background: linear-gradient(135deg, rgba(168,85,247,0.15), #201736); color: #c084fc; }

    /* Posture & Gesture Grid */
    .gesture-grid { display: grid; grid-template-columns: 1fr 1fr; gap: 10px; }
    .btn-gesture { background: #131b2e; border: 1px solid var(--card-border); color: #fff; padding: 14px 12px; border-radius: 12px; display: flex; align-items: center; gap: 12px; font-size: 13px; font-weight: 700; cursor: pointer; transition: all 0.15s ease; user-select: none; }
    .btn-gesture .g-icon { font-size: 24px; }
    .btn-gesture .g-info { text-align: left; }
    .btn-gesture .g-sub { font-size: 10px; color: var(--text-dim); font-weight: 500; }
    .btn-gesture:hover { border-color: var(--cyan); background: #18233a; transform: translateY(-2px); box-shadow: 0 4px 14px var(--cyan-glow); }
    .btn-gesture:active { transform: scale(0.97); }

    .btn-stand { border-left: 4px solid var(--green); }
    .btn-sit { border-left: 4px solid var(--yellow); }
    .btn-shake { border-left: 4px solid var(--cyan); }
    .btn-wave { border-left: 4px solid var(--purple); }

    /* Parameters & Quick Bar */
    .params-card { background: #101728; border: 1px solid var(--card-border); border-radius: 14px; padding: 14px 18px; margin-bottom: 18px; display: flex; align-items: center; justify-content: space-between; flex-wrap: wrap; gap: 14px; }
    .param-group { display: flex; align-items: center; gap: 8px; }
    .param-label { font-size: 11px; font-weight: 700; color: var(--text-dim); text-transform: uppercase; letter-spacing: 0.5px; }
    
    .pill-selector { display: flex; gap: 4px; background: rgba(0,0,0,0.3); padding: 3px; border-radius: 8px; border: 1px solid var(--card-border); }
    .pill-btn { background: transparent; border: none; color: var(--text-dim); padding: 4px 10px; border-radius: 6px; font-size: 11px; font-weight: 700; cursor: pointer; transition: all 0.15s ease; }
    .pill-btn.active { background: var(--cyan); color: #000; font-weight: 800; box-shadow: 0 0 8px var(--cyan-glow); }

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
      <img src=")rawliteral" ARK_LOGO_SRC R"rawliteral(" class="brand-logo" alt="ARK-BOT Logo" onclick="location.href='/'">
      <div class="title">
        <h1>ARK-BOT</h1>
        <p>Cyber Motion Commander (v1.0.6)</p>
      </div>
    </div>

    <!-- Navigation Tabs -->
    <nav class="nav-tabs">
      <a href="/" class="nav-tab active">🎮 Commander</a>
      <a href="/calib" class="nav-tab">🎯 Calibrator</a>
      <a href="/setup" class="nav-tab">⚙️ Setup</a>
      <a href="/update" class="nav-tab">🚀 Update</a>
    </nav>
    
    <div class="header-right">
      <!-- Battery Widget -->
      <div class="rf-widget" title="Live Battery Voltage (Xiao Expansion A0)">
        <span class="rf-label">🔋 BAT:</span>
        <span class="rssi-badge" id="vbatPill" style="color:var(--cyan); font-weight:800;">-- V</span>
      </div>

      <!-- Antenna Control -->
      <div class="rf-widget">
        <span class="rf-label">Antenna:</span>
        <div class="rf-btns">
          <button class="rf-btn active" id="btnAntInt" onclick="setAntenna('internal')" title="Switch to Onboard Ceramic Antenna">📡 Int</button>
          <button class="rf-btn" id="btnAntExt" onclick="setAntenna('external')" title="Switch to External IPEX/U.FL Antenna">🛰️ Ext</button>
        </div>
        <span class="rssi-badge" id="rssiVal">📶 -- dBm</span>
      </div>
      
      <div id="botStatus" class="badge badge-online">STATUS: READY</div>
    </div>
  </header>

  <!-- Visual Animated Robot Simulation & IMU Attitude Grid -->
  <div class="tele-sim-grid">
    <!-- Left: 3D Kinematics Visualizer -->
    <div class="sim-card">
      <div class="sim-topbar">
        <div class="sim-title">
          <span>⚡ Live Kinematics</span>
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
        <div class="sim-status" id="simPoseText">Pose: STANDING NEUTRAL (Z = -100mm)</div>
        <div id="activeActionText" style="color:var(--text-dim); font-weight:600;">Action: IDLE</div>
      </div>
    </div>

    <!-- Right: Artificial Horizon (MPU6050 6-Axis IMU) -->
    <div class="sim-card horizon-panel">
      <div class="sim-topbar" style="width:100%;">
        <div class="sim-title">
          <span>🧭 Artificial Horizon</span>
        </div>
        <button class="rf-btn" onclick="calibrateImu()" title="Tare Level Zero Offsets (Save to NVS)">🎯 Zero IMU</button>
      </div>

      <div class="horizon-canvas-wrap">
        <canvas id="horizonCanvas" width="180" height="180"></canvas>
      </div>

      <div class="horizon-metrics">
        <div class="h-metric-item">
          <span class="h-metric-label">PITCH</span>
          <span class="h-metric-val" id="imuPitchVal">0.0°</span>
        </div>
        <div class="h-metric-item">
          <span class="h-metric-label">ROLL</span>
          <span class="h-metric-val" id="imuRollVal">0.0°</span>
        </div>
      </div>

      <div class="h-accel-row">
        <span>G-FORCE: <b id="imuAx">0.00</b>g, <b id="imuAy">0.00</b>g, <b id="imuAz">1.00</b>g</span>
      </div>
    </div>
  </div>

  <!-- Parameters Bar (Steps & Speed) -->
  <div class="params-card">
    <div class="param-group">
      <span class="param-label">Step Count:</span>
      <div class="pill-selector">
        <button class="pill-btn" onclick="setStepCount(1)">1</button>
        <button class="pill-btn" onclick="setStepCount(2)">2</button>
        <button class="pill-btn active" onclick="setStepCount(3)">3</button>
        <button class="pill-btn" onclick="setStepCount(5)">5</button>
        <button class="pill-btn" onclick="setStepCount(10)">10</button>
      </div>
    </div>

    <div class="param-group">
      <span class="param-label">Gait Speed:</span>
      <div class="pill-selector">
        <button class="pill-btn active" onclick="setSpeed(1.0)">1.0x Normal</button>
        <button class="pill-btn" onclick="setSpeed(1.5)">1.5x Fast</button>
        <button class="pill-btn" onclick="setSpeed(2.0)">2.0x Turbo</button>
      </div>
    </div>
  </div>

  <div class="commander-grid">
    <!-- Card 1: Directional Gait D-Pad -->
    <div class="card">
      <div class="card-title">
        <span>🕹️ Locomotion & Turning</span>
        <div class="pill-selector">
          <button class="pill-btn" id="btnHgtLow" onclick="setWalkHeight('low')">🔻 Low (-80mm)</button>
          <button class="pill-btn active" id="btnHgtNorm" onclick="setWalkHeight('normal')">🚶 Normal (-100mm)</button>
          <button class="pill-btn" id="btnHgtHigh" onclick="setWalkHeight('high')">🔺 High (-125mm)</button>
        </div>
      </div>
      <div class="dpad-container">
        <div class="dpad-row">
          <button class="btn-action fwd" onclick="sendAction('forward')">
            <span class="icon">⬆️</span>
            <span>FORWARD</span>
          </button>
        </div>
        <div class="dpad-row">
          <button class="btn-action turn" onclick="sendAction('turn_left')">
            <span class="icon">↺</span>
            <span>TURN L</span>
          </button>
          <button class="btn-action stop" onclick="sendAction('stop')">
            <span class="icon">🛑</span>
            <span>STOP / REST</span>
          </button>
          <button class="btn-action turn" onclick="sendAction('turn_right')">
            <span class="icon">↻</span>
            <span>TURN R</span>
          </button>
        </div>
        <div class="dpad-row">
          <button class="btn-action back" onclick="sendAction('backward')">
            <span class="icon">⬇️</span>
            <span>BACKWARD</span>
          </button>
        </div>
      </div>
    </div>

    <!-- Card 2: Posture & Social Gestures -->
    <div class="card">
      <div class="card-title">
        <span>🤖 Postures & Gestures</span>
      </div>
      <div class="gesture-grid">
        <button class="btn-gesture btn-stand" onclick="sendAction('stand')">
          <span class="g-icon">🧍</span>
          <div class="g-info">
            <div>Stand Normal</div>
            <div class="g-sub">Height -100mm</div>
          </div>
        </button>

        <button class="btn-gesture btn-stand" style="border-left-color: #14b8a6;" onclick="sendAction('stand_high')">
          <span class="g-icon">🦒</span>
          <div class="g-info">
            <div>Stand High</div>
            <div class="g-sub">Height -130mm</div>
          </div>
        </button>

        <button class="btn-gesture btn-sit" onclick="sendAction('sit')">
          <span class="g-icon">🧘</span>
          <div class="g-info">
            <div>Sit Down</div>
            <div class="g-sub">Rest Height -56mm</div>
          </div>
        </button>

        <button class="btn-gesture btn-shake" onclick="sendAction('hand_shake')">
          <span class="g-icon">🤝</span>
          <div class="g-info">
            <div>Hand Shake</div>
            <div class="g-sub">Tilt & Greet</div>
          </div>
        </button>

        <button class="btn-gesture btn-wave" onclick="sendAction('hand_wave')">
          <span class="g-icon">👋</span>
          <div class="g-info">
            <div>Hand Wave</div>
            <div class="g-sub">Wave Paw</div>
          </div>
        </button>
      </div>
    </div>
  </div>

  <!-- App Footer -->
  <footer class="app-footer">
    <div class="footer-inner">
      <div class="footer-brand">
        <span class="footer-dot"></span>
        <span class="footer-title">ARK-BOT SYSTEM &bull; v1.0.6</span>
      </div>
      <div class="footer-credit">
        Designed & Engineered by <span class="author-name">Amuthesan</span>
      </div>
      <div class="footer-org">Ark Technology</div>
    </div>
  </footer>

  <div id="toast">Command Sent</div>

  <script>
    let currentSteps = 3;
    let currentSpeedMult = 1.0;
    let currentWalkHgt = 'normal';
    let viewMode = 'iso';
    let isExecuting = false;
    let pollInterval = 300;
    let pollTimer = null;

    // IMU State
    let imuReady = false;
    let targetPitch = 0.0, currentSimPitch = 0.0;
    let targetRoll = 0.0, currentSimRoll = 0.0;
    let imuAx = 0.0, imuAy = 0.0, imuAz = 1.0;

    // Real-time Cartesian Sites [x, y, z] for 4 legs: 0:FR, 1:RR, 2:FL, 3:RL
    let targetSites = [
      [124, 80, -56],  // Leg 0 (FR)
      [124, 80, -56],  // Leg 1 (RR)
      [124, 0, -56],   // Leg 2 (FL)
      [124, 0, -56]    // Leg 3 (RL)
    ];
    let currentSimSites = [
      [124, 80, -56],
      [124, 80, -56],
      [124, 0, -56],
      [124, 0, -56]
    ];
    let targetAngles = Array(4).fill().map(() => Array(3).fill(90));
    let currentSimAngles = Array(4).fill().map(() => Array(3).fill(90));

    const botLogoImg = new Image();
    botLogoImg.src = ")rawliteral" ARK_LOGO_SRC R"rawliteral(";

    function setStepCount(cnt) {
      currentSteps = cnt;
      document.querySelectorAll('.param-group:first-child .pill-btn').forEach(b => {
        b.classList.toggle('active', parseInt(b.innerText) === cnt);
      });
      showToast(`Step Count: ${cnt}`);
    }

    function setSpeed(spd) {
      currentSpeedMult = spd;
      document.querySelectorAll('.param-group:last-child .pill-btn').forEach(b => {
        b.classList.toggle('active', parseFloat(b.innerText) === spd);
      });
      showToast(`Gait Speed: ${spd}x`);
    }

    async function setWalkHeight(hgt) {
      currentWalkHgt = hgt;
      updateWalkHeightUI(hgt);
      showToast(`Walking Height: ${hgt.toUpperCase()}`);
      try {
        const res = await fetch(`/api/action?action=set_height_${hgt}&height=${hgt}`, { method: 'POST' });
        const data = await res.json();
        if (data.sites) updateSites(data.sites);
        if (data.angles) updateAngles(data.angles);
      } catch (err) {
        console.error("Set height error", err);
      }
    }

    function updateWalkHeightUI(hgt) {
      if (!hgt) return;
      const bLow = document.getElementById('btnHgtLow');
      const bNorm = document.getElementById('btnHgtNorm');
      const bHigh = document.getElementById('btnHgtHigh');
      if (bLow && bNorm && bHigh) {
        bLow.className = (hgt === 'low') ? 'pill-btn active' : 'pill-btn';
        bNorm.className = (hgt === 'normal') ? 'pill-btn active' : 'pill-btn';
        bHigh.className = (hgt === 'high') ? 'pill-btn active' : 'pill-btn';
      }
    }

    function setViewMode(mode) {
      viewMode = mode;
      ['Iso', 'Top', 'Side'].forEach(m => {
        const b = document.getElementById(`btnView${m}`);
        if (b) b.className = (m.toLowerCase() === mode) ? 'view-btn active' : 'view-btn';
      });
    }

    let lastPcaReady = false;

    async function sendAction(act) {
      if (!lastPcaReady && act !== 'stop') {
        showToast("⚠️ PCA9685 Driver Offline! Check I2C / 5V power.");
        return;
      }

      const stepArg = (act === 'stand') ? 1 : (act === 'sit' ? 0 : currentSteps);
      showToast(`Executing: ${act.toUpperCase()} (${stepArg} steps)...`);
      
      const botStatus = document.getElementById('botStatus');
      const actionText = document.getElementById('activeActionText');
      if (botStatus) {
        botStatus.className = 'badge badge-busy';
        botStatus.innerText = 'STATUS: MOVING';
      }
      if (actionText) actionText.innerText = `Action: ${act.toUpperCase()}`;

      setFastPolling(true);

      try {
        const res = await fetch(`/api/action?action=${act}&steps=${stepArg}&speed=${currentSpeedMult}`, { method: 'POST' });
        const data = await res.json();
        if (data.sites) updateSites(data.sites);
        if (data.angles) updateAngles(data.angles);
      } catch (err) {
        console.error("Action error", err);
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

    async function calibrateImu() {
      showToast("🎯 Zeroing IMU... Keep robot stationary!");
      try {
        const res = await fetch('/api/imu/calibrate', { method: 'POST' });
        const data = await res.json();
        if (data.success) {
          showToast(`✅ IMU Calibrated! P Off: ${data.pitch_offset.toFixed(1)}° | R Off: ${data.roll_offset.toFixed(1)}°`);
          fetchStatus();
        } else {
          showToast("⚠️ IMU calibration failed");
        }
      } catch (err) {
        console.error("IMU calibrate error", err);
        showToast("⚠️ IMU calibration error");
      }
    }

    async function fetchStatus() {
      try {
        const res = await fetch('/api/status');
        const data = await res.json();
        lastPcaReady = !!data.pcaReady;

        if (data.sites) updateSites(data.sites);
        if (data.angles) updateAngles(data.angles);
        if (data.extAntenna !== undefined) updateAntennaUI(data.extAntenna, data.rssi);
        if (data.walk_height) updateWalkHeightUI(data.walk_height);
        
        if (data.vbat !== undefined) {
          const vbatPill = document.getElementById('vbatPill');
          if (vbatPill) {
            vbatPill.innerText = `${data.vbat.toFixed(1)}V`;
            if (data.vbat < 6.8) vbatPill.style.color = '#ef4444';
            else if (data.vbat < 7.4) vbatPill.style.color = '#f59e0b';
            else vbatPill.style.color = '#00f0ff';
          }
        }

        // IMU Telemetry
        if (data.imu) {
          imuReady = !!data.imu.ready;
          targetPitch = data.imu.pitch || 0.0;
          targetRoll  = data.imu.roll || 0.0;
          imuAx = (data.imu.ax !== undefined) ? data.imu.ax : 0.0;
          imuAy = (data.imu.ay !== undefined) ? data.imu.ay : 0.0;
          imuAz = (data.imu.az !== undefined) ? data.imu.az : 1.0;

          const pVal = document.getElementById('imuPitchVal');
          const rVal = document.getElementById('imuRollVal');
          const axEl = document.getElementById('imuAx');
          const ayEl = document.getElementById('imuAy');
          const azEl = document.getElementById('imuAz');

          if (pVal) pVal.innerText = `${targetPitch >= 0 ? '+' : ''}${targetPitch.toFixed(1)}°`;
          if (rVal) rVal.innerText = `${targetRoll >= 0 ? '+' : ''}${targetRoll.toFixed(1)}°`;
          if (axEl) axEl.innerText = `${imuAx.toFixed(2)}`;
          if (ayEl) ayEl.innerText = `${imuAy.toFixed(2)}`;
          if (azEl) azEl.innerText = `${imuAz.toFixed(2)}`;
        }

        const botStatus = document.getElementById('botStatus');
        const actionText = document.getElementById('activeActionText');
        const simPoseText = document.getElementById('simPoseText');

        if (botStatus) {
          if (!data.pcaReady) {
            botStatus.className = 'badge badge-offline';
            botStatus.innerText = 'STATUS: PCA OFFLINE (0x40)';
            setFastPolling(false);
          } else if (data.moving) {
            botStatus.className = 'badge badge-busy';
            botStatus.innerText = 'STATUS: MOVING';
            setFastPolling(true);
          } else {
            botStatus.className = 'badge badge-online';
            botStatus.innerText = 'STATUS: READY';
            setFastPolling(false);
          }
        }
        if (actionText) {
          if (!data.pcaReady) {
            actionText.innerText = '⚠️ Warning: PCA9685 Driver Offline (0x40)';
            actionText.style.color = '#ef4444';
          } else {
            actionText.innerText = `Mode: ${data.mode || 'READY'}`;
            actionText.style.color = 'var(--text-dim)';
          }
        }
        if (simPoseText && data.sites) {
          const z0 = Math.round(data.sites[0][2]);
          simPoseText.innerText = data.pcaReady ? `Pose: ${data.mode || 'ACTIVE'} (Z ≈ ${z0} mm)` : 'Pose: HARDWARE OFFLINE';
        }
      } catch (err) {}
    }

    function setFastPolling(fast) {
      const targetInt = fast ? 90 : 350;
      if (pollInterval !== targetInt) {
        pollInterval = targetInt;
        clearInterval(pollTimer);
        pollTimer = setInterval(fetchStatus, pollInterval);
      }
    }

    function updateSites(sites) {
      for (let l = 0; l < 4; l++) {
        if (sites[l]) {
          targetSites[l][0] = sites[l][0];
          targetSites[l][1] = sites[l][1];
          targetSites[l][2] = sites[l][2];
        }
      }
    }

    function updateAngles(angles) {
      for (let l = 0; l < 4; l++) {
        for (let j = 0; j < 3; j++) {
          targetAngles[l][j] = angles[l][j];
        }
      }
    }

    function showToast(msg) {
      const toast = document.getElementById('toast');
      toast.innerText = msg;
      toast.classList.add('show');
      setTimeout(() => toast.classList.remove('show'), 2000);
    }

    // ==========================================
    // Artificial Horizon Vector Renderer
    // ==========================================
    function drawArtificialHorizon(canvasId, pitch, roll, ready) {
      const c = document.getElementById(canvasId);
      if (!c) return;
      const hCtx = c.getContext('2d');
      const w = c.width, h = c.height;
      const r = Math.min(w, h) / 2 - 4;
      const cx = w / 2, cy = h / 2;

      hCtx.clearRect(0, 0, w, h);

      // Background Bezel
      hCtx.save();
      hCtx.beginPath();
      hCtx.arc(cx, cy, r, 0, Math.PI * 2);
      hCtx.clip();

      if (!ready) {
        hCtx.fillStyle = '#0f172a';
        hCtx.fill();
        hCtx.fillStyle = '#64748b';
        hCtx.font = 'bold 12px sans-serif';
        hCtx.textAlign = 'center';
        hCtx.textBaseline = 'middle';
        hCtx.fillText('MPU6050 OFFLINE', cx, cy);
        hCtx.restore();
        return;
      }

      // Attitude Transformation: Rotate by Roll, Translate by Pitch
      hCtx.save();
      hCtx.translate(cx, cy);
      hCtx.rotate((-roll * Math.PI) / 180.0);

      const pxPerDeg = r / 35.0; // 35 degrees to outer edge
      const pitchPx = pitch * pxPerDeg;

      // Sky (Top Half - Cyber Blue)
      hCtx.fillStyle = '#0284c7';
      hCtx.fillRect(-r * 2, -r * 2 + pitchPx, r * 4, r * 2);

      // Ground (Bottom Half - Earth Slate / Warm Umber)
      hCtx.fillStyle = '#78350f';
      hCtx.fillRect(-r * 2, pitchPx, r * 4, r * 2);

      // Horizon Center Line
      hCtx.strokeStyle = '#ffffff';
      hCtx.lineWidth = 2.5;
      hCtx.beginPath();
      hCtx.moveTo(-r * 2, pitchPx);
      hCtx.lineTo(r * 2, pitchPx);
      hCtx.stroke();

      // Pitch Ladder Lines
      hCtx.strokeStyle = 'rgba(255, 255, 255, 0.85)';
      hCtx.fillStyle = '#ffffff';
      hCtx.font = 'bold 9px monospace';
      hCtx.textAlign = 'center';
      hCtx.textBaseline = 'middle';

      for (let deg = -40; deg <= 40; deg += 10) {
        if (deg === 0) continue;
        const lineY = pitchPx - (deg * pxPerDeg);
        const lineW = (Math.abs(deg) % 20 === 0) ? 26 : 14;

        hCtx.lineWidth = (Math.abs(deg) % 20 === 0) ? 2 : 1.2;
        hCtx.beginPath();
        hCtx.moveTo(-lineW, lineY);
        hCtx.lineTo(lineW, lineY);
        hCtx.stroke();

        if (Math.abs(deg) % 10 === 0) {
          hCtx.fillText(Math.abs(deg).toString(), lineW + 8, lineY);
          hCtx.fillText(Math.abs(deg).toString(), -lineW - 8, lineY);
        }
      }

      hCtx.restore(); // Restore from roll/pitch transform

      // Outer Bezel & Bank Markers
      hCtx.restore(); // Restore clipping
      hCtx.save();
      hCtx.beginPath();
      hCtx.arc(cx, cy, r, 0, Math.PI * 2);
      hCtx.lineWidth = 3.5;
      hCtx.strokeStyle = '#00f0ff';
      hCtx.shadowColor = 'rgba(0, 240, 255, 0.5)';
      hCtx.shadowBlur = 8;
      hCtx.stroke();
      hCtx.shadowBlur = 0;

      // Bank Markers (0, +/-30, +/-60 deg around top arc)
      hCtx.strokeStyle = '#00f0ff';
      hCtx.lineWidth = 2;
      [0, -30, 30, -60, 60].forEach(angle => {
        const rad = ((angle - 90) * Math.PI) / 180.0;
        const x1 = cx + (r - 2) * Math.cos(rad);
        const y1 = cy + (r - 2) * Math.sin(rad);
        const x2 = cx + (r - 8) * Math.cos(rad);
        const y2 = cy + (r - 8) * Math.sin(rad);
        hCtx.beginPath(); hCtx.moveTo(x1, y1); hCtx.lineTo(x2, y2); hCtx.stroke();
      });

      // Fixed Aircraft Reticle (Center Reference Symbol)
      hCtx.strokeStyle = '#facc15';
      hCtx.fillStyle = '#facc15';
      hCtx.lineWidth = 3;
      hCtx.shadowColor = 'rgba(250, 204, 21, 0.6)';
      hCtx.shadowBlur = 6;

      // Left & Right Wings
      hCtx.beginPath();
      hCtx.moveTo(cx - 38, cy);
      hCtx.lineTo(cx - 14, cy);
      hCtx.lineTo(cx - 14, cy + 6);
      hCtx.moveTo(cx + 38, cy);
      hCtx.lineTo(cx + 14, cy);
      hCtx.lineTo(cx + 14, cy + 6);
      hCtx.stroke();

      // Center Reference Dot
      hCtx.beginPath();
      hCtx.arc(cx, cy, 3, 0, Math.PI * 2);
      hCtx.fill();

      // Top Zero Roll Indicator Pointer
      hCtx.beginPath();
      hCtx.moveTo(cx, cy - r + 3);
      hCtx.lineTo(cx - 5, cy - r + 11);
      hCtx.lineTo(cx + 5, cy - r + 11);
      hCtx.closePath();
      hCtx.fill();

      hCtx.restore();
    }

    // ==========================================
    // 3D Kinematics Visualizer Canvas
    // ==========================================
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

    // Physical dimensions & scaling
    const S = 0.38; // Coordinate Scale Factor (mm -> canvas pixels)
    const Lc = 72.5 * S;  // Coxa  (~27.5 px)
    const La = 84.0 * S;  // Femur (~31.9 px)
    const Lb = 145.0 * S; // Tibia (~55.1 px)
    const bodyHalfW = 34; // Body X half-width
    const bodyHalfL = 46; // Body Y half-length

    // Quadruped leg mount positions on chassis:
    // 0: Front-Right, 1: Rear-Right, 2: Front-Left, 3: Rear-Left
    const legMounts = [
      { sideX: 1, frontY: -1, name: "FR" },  // Leg 0: Front-Right
      { sideX: 1, frontY: 1,  name: "RR" },  // Leg 1: Rear-Right
      { sideX: -1, frontY: -1, name: "FL" }, // Leg 2: Front-Left
      { sideX: -1, frontY: 1,  name: "RL" }  // Leg 3: Rear-Left
    ];

    function renderKinematics() {
      const w = canvas.parentElement.clientWidth;
      const h = canvas.parentElement.clientHeight;
      ctx.clearRect(0, 0, w, h);

      // Smooth interpolation toward target sites, angles, and attitude
      for (let l = 0; l < 4; l++) {
        for (let k = 0; k < 3; k++) {
          currentSimSites[l][k] += (targetSites[l][k] - currentSimSites[l][k]) * 0.32;
          currentSimAngles[l][k] += (targetAngles[l][k] - currentSimAngles[l][k]) * 0.32;
        }
      }
      currentSimPitch += (targetPitch - currentSimPitch) * 0.25;
      currentSimRoll  += (targetRoll  - currentSimRoll)  * 0.25;

      // Draw Artificial Horizon
      drawArtificialHorizon('horizonCanvas', currentSimPitch, currentSimRoll, imuReady);

      const cx = w / 2;
      const cy = h / 2 + (viewMode === 'side' ? 20 : 0);

      // Dynamic Body Elevation from average stance height
      const avgZ = (currentSimSites[0][2] + currentSimSites[1][2] + currentSimSites[2][2] + currentSimSites[3][2]) / 4;
      const bodyElevZ = -avgZ * S * 0.75; // Elevate body above floor

      // 3D Attitude Rotation (Roll & Pitch)
      const phi = (currentSimRoll * Math.PI) / 180.0;
      const theta = (currentSimPitch * Math.PI) / 180.0;

      function rotateBody(x, y, z) {
        // Roll around X-axis / Pitch around Y-axis
        const rx = x * Math.cos(phi) - (z - bodyElevZ) * Math.sin(phi);
        const ry = y * Math.cos(theta) - (z - bodyElevZ) * Math.sin(theta);
        const rz = bodyElevZ + (z - bodyElevZ) + x * Math.sin(phi) + y * Math.sin(theta);
        return { x: rx, y: ry, z: rz };
      }

      // 3D Point Projection Helper
      function projectPoint(x, y, z) {
        if (viewMode === 'top') {
          return { x: cx + x * 1.55, y: cy + y * 1.55 };
        }
        if (viewMode === 'side') {
          return { x: cx + y * 1.7, y: cy - z * 1.5 };
        }
        // 3D Isometric Projection
        const isoX = (x - y) * Math.cos(Math.PI / 6);
        const isoY = (x + y) * Math.sin(Math.PI / 6) - z;
        return { x: cx + isoX * 1.28, y: cy + isoY * 1.28 + 12 };
      }

      // Floor Grid
      ctx.strokeStyle = "rgba(0, 240, 255, 0.05)";
      ctx.lineWidth = 1;
      const gridSize = 28;
      for (let x = 0; x < w; x += gridSize) {
        ctx.beginPath(); ctx.moveTo(x, 0); ctx.lineTo(x, h); ctx.stroke();
      }
      for (let y = 0; y < h; y += gridSize) {
        ctx.beginPath(); ctx.moveTo(0, y); ctx.lineTo(w, y); ctx.stroke();
      }

      // Compute 3D coordinates for all 4 legs using Exact Forward Kinematics
      const legPoints = [];
      for (let l = 0; l < 4; l++) {
        const [x_mm, y_mm, z_mm] = currentSimSites[l];
        const m = legMounts[l];

        // 1. Hip Mount Origin on Body Chassis (Rotated with IMU Attitude)
        const unrotHipX = m.sideX * bodyHalfW;
        const unrotHipY = m.frontY * bodyHalfL;
        const rotHip = rotateBody(unrotHipX, unrotHipY, bodyElevZ);

        const hipX = rotHip.x;
        const hipY = rotHip.y;
        const hipZ = rotHip.z;

        // 2. Exact Inverse-to-Forward Trigonometry (Kinematics Vector Math)
        const w_mm = Math.sqrt(x_mm * x_mm + y_mm * y_mm);
        const v_mm = w_mm - 72.5; // distance beyond Coxa
        const gamma = (w_mm >= 0) ? Math.atan2(y_mm, x_mm) : Math.atan2(-y_mm, -x_mm);

        const v_px = v_mm * S;
        const z_px = z_mm * S;
        const L_px = Math.sqrt(v_px * v_px + z_px * z_px);
        const ang0 = Math.atan2(z_px, Math.max(0.01, v_px));
        let cosA = (La * La - Lb * Lb + L_px * L_px) / (2 * La * Math.max(0.01, L_px));
        cosA = Math.max(-1, Math.min(1, cosA));
        const alpha = ang0 + Math.acos(cosA);

        // 3. 3D Joint Locations
        const coxaX = hipX + m.sideX * (Lc * Math.cos(gamma));
        const coxaY = hipY - (Lc * Math.sin(gamma));
        const coxaZ = hipZ;

        const kneeX = coxaX + m.sideX * (La * Math.cos(alpha) * Math.cos(gamma));
        const kneeY = coxaY - (La * Math.cos(alpha) * Math.sin(gamma));
        const kneeZ = coxaZ + (La * Math.sin(alpha));

        const footX = hipX + m.sideX * (x_mm * S);
        const footY = hipY - ((y_mm - 40) * S);
        const footZ = hipZ + (z_mm * S);

        legPoints.push({
          p0: projectPoint(hipX, hipY, hipZ),
          p1: projectPoint(coxaX, coxaY, coxaZ),
          p2: projectPoint(kneeX, kneeY, kneeZ),
          p3: projectPoint(footX, footY, footZ),
          footZ: footZ,
          mount: m
        });
      }

      // Ground Shadows under feet
      legPoints.forEach(leg => {
        ctx.beginPath();
        ctx.ellipse(leg.p3.x, leg.p3.y + 12, 10, 5, 0, 0, Math.PI * 2);
        ctx.fillStyle = "rgba(0, 0, 0, 0.45)";
        ctx.fill();
      });

      // Render 4 Quadruped Limbs
      legPoints.forEach((leg, l) => {
        // Link 1: Hip to Coxa (Cyan Rod)
        ctx.beginPath();
        ctx.moveTo(leg.p0.x, leg.p0.y);
        ctx.lineTo(leg.p1.x, leg.p1.y);
        ctx.strokeStyle = "#00f0ff";
        ctx.lineWidth = 5;
        ctx.lineCap = "round";
        ctx.stroke();

        // Hip & Coxa Joint Spheres
        ctx.beginPath(); ctx.arc(leg.p0.x, leg.p0.y, 4, 0, Math.PI * 2); ctx.fillStyle = "#38bdf8"; ctx.fill();
        ctx.beginPath(); ctx.arc(leg.p1.x, leg.p1.y, 4, 0, Math.PI * 2); ctx.fillStyle = "#0284c7"; ctx.fill();

        // Link 2: Femur Thigh (Deep Cyber Blue)
        ctx.beginPath();
        ctx.moveTo(leg.p1.x, leg.p1.y);
        ctx.lineTo(leg.p2.x, leg.p2.y);
        ctx.strokeStyle = "#3b82f6";
        ctx.lineWidth = 4;
        ctx.stroke();

        // Knee Joint Sphere
        ctx.beginPath(); ctx.arc(leg.p2.x, leg.p2.y, 4.5, 0, Math.PI * 2);
        ctx.fillStyle = "#60a5fa";
        ctx.fill();

        // Link 3: Tibia Calf (Bright Neon Cyan)
        ctx.beginPath();
        ctx.moveTo(leg.p2.x, leg.p2.y);
        ctx.lineTo(leg.p3.x, leg.p3.y);
        ctx.strokeStyle = "#00f0ff";
        ctx.lineWidth = 3.5;
        ctx.stroke();

        // Glowing Foot Tip
        ctx.beginPath();
        ctx.arc(leg.p3.x, leg.p3.y, 5.5, 0, Math.PI * 2);
        ctx.fillStyle = "#10b981";
        ctx.shadowColor = "rgba(16, 185, 129, 0.9)";
        ctx.shadowBlur = 10;
        ctx.fill();
        ctx.shadowBlur = 0;
      });

      // Chassis Body Shell (Rotated with IMU Attitude)
      const rot_fr = rotateBody(bodyHalfW, -bodyHalfL, bodyElevZ + 8);
      const rot_rr = rotateBody(bodyHalfW, bodyHalfL, bodyElevZ + 8);
      const rot_rl = rotateBody(-bodyHalfW, bodyHalfL, bodyElevZ + 8);
      const rot_fl = rotateBody(-bodyHalfW, -bodyHalfL, bodyElevZ + 8);

      const c_fr = projectPoint(rot_fr.x, rot_fr.y, rot_fr.z);
      const c_rr = projectPoint(rot_rr.x, rot_rr.y, rot_rr.z);
      const c_rl = projectPoint(rot_rl.x, rot_rl.y, rot_rl.z);
      const c_fl = projectPoint(rot_fl.x, rot_fl.y, rot_fl.z);

      ctx.save();
      ctx.beginPath();
      ctx.moveTo(c_fl.x, c_fl.y);
      ctx.lineTo(c_fr.x, c_fr.y);
      ctx.lineTo(c_rr.x, c_rr.y);
      ctx.lineTo(c_rl.x, c_rl.y);
      ctx.closePath();
      
      const bodyGrad = ctx.createLinearGradient(c_fl.x, c_fl.y, c_rr.x, c_rr.y);
      bodyGrad.addColorStop(0, "#1e293b");
      bodyGrad.addColorStop(0.5, "#0f172a");
      bodyGrad.addColorStop(1, "#1e293b");
      ctx.fillStyle = bodyGrad;
      ctx.fill();
      ctx.lineWidth = 2;
      ctx.strokeStyle = "rgba(0, 240, 255, 0.6)";
      ctx.shadowColor = "rgba(0, 240, 255, 0.4)";
      ctx.shadowBlur = 8;
      ctx.stroke();
      ctx.shadowBlur = 0;

      // Chassis Center Core Logo
      const rot_core = rotateBody(0, 0, bodyElevZ + 8);
      const coreP = projectPoint(rot_core.x, rot_core.y, rot_core.z);
      if (botLogoImg.complete && botLogoImg.naturalWidth > 0) {
        const sz = 32;
        ctx.drawImage(botLogoImg, coreP.x - sz/2, coreP.y - sz/2, sz, sz);
      }
      ctx.restore();

      requestAnimationFrame(renderKinematics);
    }

    fetchStatus();
    pollTimer = setInterval(fetchStatus, pollInterval);
    requestAnimationFrame(renderKinematics);
  </script>
</body>
</html>
)rawliteral";

// Alias
#define INDEX_HTML COMMANDER_HTML

// =============================================================================
// 2. CALIBRATION PAGE HTML (http://arkbot.local/calib)
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
    
    .leg-header { display: flex; justify-content: space-between; align-items: center; margin-bottom: 14px; padding-bottom: 8px; border-bottom: 1px solid rgba(255,255,255,0.06); }
    .leg-title { font-size: 15px; font-weight: 800; color: var(--cyan); display: flex; align-items: center; gap: 8px; letter-spacing: 0.5px; }
    .leg-actions { display: flex; gap: 6px; }

    /* Joint Control */
    .joint-row { margin-bottom: 12px; background: #131b2c; padding: 10px 12px; border-radius: 10px; border: 1px solid rgba(255,255,255,0.04); }
    .joint-row:last-child { margin-bottom: 0; }
    .joint-row.joint-disabled { opacity: 0.5; }
    .joint-row:hover { border-color: rgba(0,240,255,0.3); }

    .joint-top { display: flex; justify-content: space-between; align-items: center; margin-bottom: 8px; font-size: 13px; }
    .joint-left { display: flex; align-items: center; gap: 8px; }
    .joint-name { font-weight: 700; color: var(--text-main); }
    .joint-ch { font-size: 10px; font-weight: 700; color: var(--cyan); background: #1b263b; padding: 2px 7px; border-radius: 4px; border: 1px solid rgba(0,240,255,0.2); }
    
    .joint-right { display: flex; align-items: center; gap: 8px; }
    .joint-val { font-family: ui-monospace, SFMono-Regular, monospace; font-weight: 800; color: var(--cyan); background: #090d16; padding: 3px 8px; border-radius: 6px; border: 1px solid #1e293b; font-size: 13px; min-width: 48px; text-align: center; }

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

    #toast { position: fixed; bottom: 24px; left: 50%; transform: translateX(-50%); background: #10b981; color: white; padding: 10px 22px; border-radius: 30px; font-size: 13px; font-weight: 700; box-shadow: 0 8px 24px rgba(0,0,0,0.6); opacity: 0; pointer-events: none; transition: opacity 0.2s ease; z-index: 100; border: 1px solid rgba(255,255,255,0.2); }
    #toast.show { opacity: 1; }
  </style>
</head>
<body>

  <header>
    <div class="brand">
      <img src=")rawliteral" ARK_LOGO_SRC R"rawliteral(" class="brand-logo" alt="ARK-BOT Logo" onclick="location.href='/'">
      <div class="title">
        <h1>ARK-BOT</h1>
        <p>12-Servo Kinematics Calibrator</p>
      </div>
    </div>

    <!-- Navigation Tabs -->
    <nav class="nav-tabs">
      <a href="/" class="nav-tab">🎮 Commander</a>
      <a href="/calib" class="nav-tab active">🎯 Calibrator</a>
      <a href="/setup" class="nav-tab">⚙️ Setup</a>
      <a href="/update" class="nav-tab">🚀 Update</a>
    </nav>
    
    <div class="header-right">
      <!-- Battery Widget -->
      <div class="rf-widget" title="Live Battery Voltage (Xiao Expansion A0)">
        <span class="rf-label">🔋 BAT:</span>
        <span class="rssi-badge" id="vbatPill" style="color:var(--cyan); font-weight:800;">-- V</span>
      </div>

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

  <!-- Master Controls -->
  <div class="section-title">Master Power & Alignment</div>
  <div class="master-grid">
    <button class="btn btn-green" onclick="setMasterPower(1)">⚡ Start All Servos</button>
    <button class="btn btn-red" onclick="setMasterPower(0)">🛑 Stop All (Release)</button>
    <button class="btn btn-cyan" onclick="sendInit('all')">🎯 Center All (90°)</button>
    <button class="btn btn-dark" onclick="sendInit('wave')">🌊 Wave Calibration</button>
    <button class="btn btn-dark" onclick="sendBeep()">🔔 Beep Buzzer</button>
  </div>

  <!-- MPU6050 6-Axis IMU & Attitude Calibration Card -->
  <div class="section-title">MPU6050 6-Axis IMU & Attitude Tare</div>
  <div class="card" style="margin-bottom:18px; padding:16px;">
    <div style="display:flex; justify-content:space-between; align-items:center; flex-wrap:wrap; gap:16px;">
      <div style="display:flex; align-items:center; gap:18px; flex-wrap:wrap;">
        <canvas id="calibHorizonCanvas" width="130" height="130" style="width:130px; height:130px; border-radius:50%; box-shadow:0 0 16px rgba(0,240,255,0.25);"></canvas>
        <div>
          <div style="font-size:15px; font-weight:800; color:var(--cyan); margin-bottom:4px;">Attitude Director Indicator (IMU)</div>
          <div style="font-size:11px; color:var(--text-dim); margin-bottom:8px;">50Hz Complementary Filter Fusion (0x68 / 0x69)</div>
          <div style="display:flex; gap:16px; font-family:ui-monospace,SFMono-Regular,monospace; font-size:14px; font-weight:800;">
            <span style="color:#ffffff;">PITCH: <span id="calibPitch" style="color:var(--cyan);">0.0°</span></span>
            <span style="color:#ffffff;">ROLL: <span id="calibRoll" style="color:var(--cyan);">0.0°</span></span>
          </div>
          <div style="font-size:11px; color:var(--text-dim); margin-top:4px; font-family:ui-monospace,SFMono-Regular,monospace;" id="calibGForce">
            ACCEL: 0.00g, 0.00g, 1.00g
          </div>
        </div>
      </div>
      <div style="display:flex; flex-direction:column; gap:8px; align-items:flex-end;">
        <button class="btn btn-cyan" onclick="calibrateImu()">🎯 Tare Level Zero (Save to NVS)</button>
        <div id="calibOffsets" style="font-size:11px; color:var(--text-dim);">Offsets: P 0.0° | R 0.0°</div>
      </div>
    </div>
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
        <span class="footer-title">ARK-BOT SYSTEM &bull; v1.0.6</span>
      </div>
      <div class="footer-credit">
        Designed & Engineered by <span class="author-name">Amuthesan</span>
      </div>
      <div class="footer-org">Ark Technology</div>
    </div>
  </footer>

  <div id="toast">Command Sent</div>

  <script>
    const LEGS = ["Front-Right (FR)", "Rear-Right (RR)", "Front-Left (FL)", "Rear-Left (RL)"];
    const SHORT_LEGS = ["FR", "RR", "FL", "RL"];
    const JOINTS = ["Coxa (Hip)", "Femur (Thigh)", "Tibia (Calf)"];
    const CHANNELS = [
      [4, 2, 3],
      [7, 5, 6],
      [10, 8, 9],
      [13, 11, 12]
    ];
    
    let servoPower = Array(4).fill().map(() => Array(3).fill(true));
    let imuReady = false;
    let currentPitch = 0.0, currentRoll = 0.0;

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
                  <button class="btn-sm" onclick="setJoint(${l}, ${j}, 0)">0°</button>
                  <button class="btn-sm" onclick="setJoint(${l}, ${j}, 90)">90°</button>
                  <button class="btn-sm" onclick="setJoint(${l}, ${j}, 180)">180°</button>
                  <button class="btn-sm" style="color:var(--cyan)" title="Init this joint" onclick="setJoint(${l}, ${j}, 90)">⚡</button>
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
      if (!throttleTimer) {
        throttleTimer = setTimeout(() => {
          sendServoAngle(leg, joint, angle);
          throttleTimer = null;
        }, 50);
      }
    }

    function onSliderChange(leg, joint, angle) {
      document.getElementById(`val_${leg}_${joint}`).innerText = `${angle}°`;
      sendServoAngle(leg, joint, angle);
    }

    function setJoint(leg, joint, angle) {
      document.getElementById(`slider_${leg}_${joint}`).value = angle;
      document.getElementById(`val_${leg}_${joint}`).innerText = `${angle}°`;
      sendServoAngle(leg, joint, angle);
    }

    function initLeg(leg) {
      for (let j = 0; j < 3; j++) setJoint(leg, j, 90);
      showToast(`Initialized Leg ${SHORT_LEGS[leg]} to 90°`);
    }

    async function sendServoAngle(leg, joint, angle) {
      try {
        await fetch(`/api/servo?leg=${leg}&joint=${joint}&angle=${angle}`, { method: 'POST' });
      } catch (err) {}
    }

    async function setAntenna(type) {
      showToast(`Switching to ${type.toUpperCase()} Antenna...`);
      try {
        const res = await fetch(`/api/antenna?type=${type}`, { method: 'POST' });
        const data = await res.json();
        updateAntennaUI(data.extAntenna, data.rssi);
        showToast(`Antenna: ${data.extAntenna ? 'EXTERNAL (IPEX)' : 'INTERNAL (Ceramic)'}`);
      } catch (err) {}
    }

    function updateAntennaUI(isExt, rssi) {
      const btnInt = document.getElementById('btnAntInt');
      const btnExt = document.getElementById('btnAntExt');
      const rssiBadge = document.getElementById('rssiVal');
      if (btnInt && btnExt) {
        btnInt.className = isExt ? 'rf-btn' : 'rf-btn active';
        btnExt.className = isExt ? 'rf-btn active' : 'rf-btn';
      }
      if (rssiBadge && rssi !== undefined) rssiBadge.innerText = `📶 ${rssi} dBm`;
    }

    async function setMasterPower(state) {
      showToast(state ? "Energizing All Servos..." : "Releasing All Servos...");
      try {
        await fetch(`/api/power?target=all&state=${state}`, { method: 'POST' });
        fetchStatus();
      } catch (err) {}
    }

    async function toggleLegPower(leg) {
      const anyOn = servoPower[leg].some(p => p);
      try {
        await fetch(`/api/power?target=leg&leg=${leg}&state=${anyOn ? 0 : 1}`, { method: 'POST' });
        fetchStatus();
      } catch (err) {}
    }

    async function toggleJointPower(leg, joint) {
      const newState = !servoPower[leg][joint];
      try {
        await fetch(`/api/power?target=joint&leg=${leg}&joint=${joint}&state=${newState ? 1 : 0}`, { method: 'POST' });
        fetchStatus();
      } catch (err) {}
    }

    async function sendInit(type) {
      showToast(type === 'wave' ? "Wave Calibration..." : "Centering All Servos (90°)...");
      try {
        await fetch(`/api/init?type=${type}`, { method: 'POST' });
        fetchStatus();
      } catch (err) {}
    }

    async function sendBeep() {
      try {
        await fetch('/api/beep', { method: 'POST' });
        showToast("Buzzer Beeped 🔔");
      } catch (err) {}
    }

    async function calibrateImu() {
      showToast("🎯 Zeroing IMU... Keep robot stationary!");
      try {
        const res = await fetch('/api/imu/calibrate', { method: 'POST' });
        const data = await res.json();
        if (data.success) {
          showToast(`✅ IMU Calibrated! P Off: ${data.pitch_offset.toFixed(1)}° | R Off: ${data.roll_offset.toFixed(1)}°`);
          fetchStatus();
        } else {
          showToast("⚠️ IMU calibration failed");
        }
      } catch (err) {
        showToast("⚠️ IMU calibration error");
      }
    }

    function drawArtificialHorizon(canvasId, pitch, roll, ready) {
      const c = document.getElementById(canvasId);
      if (!c) return;
      const hCtx = c.getContext('2d');
      const w = c.width, h = c.height;
      const r = Math.min(w, h) / 2 - 4;
      const cx = w / 2, cy = h / 2;

      hCtx.clearRect(0, 0, w, h);
      hCtx.save();
      hCtx.beginPath();
      hCtx.arc(cx, cy, r, 0, Math.PI * 2);
      hCtx.clip();

      if (!ready) {
        hCtx.fillStyle = '#0f172a';
        hCtx.fill();
        hCtx.fillStyle = '#64748b';
        hCtx.font = 'bold 11px sans-serif';
        hCtx.textAlign = 'center';
        hCtx.textBaseline = 'middle';
        hCtx.fillText('MPU6050 OFFLINE', cx, cy);
        hCtx.restore();
        return;
      }

      hCtx.save();
      hCtx.translate(cx, cy);
      hCtx.rotate((-roll * Math.PI) / 180.0);

      const pxPerDeg = r / 35.0;
      const pitchPx = pitch * pxPerDeg;

      hCtx.fillStyle = '#0284c7';
      hCtx.fillRect(-r * 2, -r * 2 + pitchPx, r * 4, r * 2);

      hCtx.fillStyle = '#78350f';
      hCtx.fillRect(-r * 2, pitchPx, r * 4, r * 2);

      hCtx.strokeStyle = '#ffffff';
      hCtx.lineWidth = 2;
      hCtx.beginPath();
      hCtx.moveTo(-r * 2, pitchPx);
      hCtx.lineTo(r * 2, pitchPx);
      hCtx.stroke();

      hCtx.strokeStyle = 'rgba(255, 255, 255, 0.85)';
      hCtx.fillStyle = '#ffffff';
      hCtx.font = 'bold 8px monospace';
      hCtx.textAlign = 'center';
      hCtx.textBaseline = 'middle';

      for (let deg = -40; deg <= 40; deg += 10) {
        if (deg === 0) continue;
        const lineY = pitchPx - (deg * pxPerDeg);
        const lineW = (Math.abs(deg) % 20 === 0) ? 20 : 12;
        hCtx.lineWidth = (Math.abs(deg) % 20 === 0) ? 1.8 : 1.0;
        hCtx.beginPath();
        hCtx.moveTo(-lineW, lineY);
        hCtx.lineTo(lineW, lineY);
        hCtx.stroke();
        if (Math.abs(deg) % 10 === 0) {
          hCtx.fillText(Math.abs(deg).toString(), lineW + 6, lineY);
          hCtx.fillText(Math.abs(deg).toString(), -lineW - 6, lineY);
        }
      }
      hCtx.restore();

      hCtx.restore();
      hCtx.save();
      hCtx.beginPath();
      hCtx.arc(cx, cy, r, 0, Math.PI * 2);
      hCtx.lineWidth = 3;
      hCtx.strokeStyle = '#00f0ff';
      hCtx.shadowColor = 'rgba(0, 240, 255, 0.5)';
      hCtx.shadowBlur = 6;
      hCtx.stroke();
      hCtx.shadowBlur = 0;

      // Aircraft symbol
      hCtx.strokeStyle = '#facc15';
      hCtx.fillStyle = '#facc15';
      hCtx.lineWidth = 2.5;
      hCtx.beginPath();
      hCtx.moveTo(cx - 30, cy); hCtx.lineTo(cx - 10, cy); hCtx.lineTo(cx - 10, cy + 5);
      hCtx.moveTo(cx + 30, cy); hCtx.lineTo(cx + 10, cy); hCtx.lineTo(cx + 10, cy + 5);
      hCtx.stroke();
      hCtx.beginPath(); hCtx.arc(cx, cy, 2.5, 0, Math.PI * 2); hCtx.fill();
      hCtx.beginPath(); hCtx.moveTo(cx, cy - r + 2); hCtx.lineTo(cx - 4, cy - r + 9); hCtx.lineTo(cx + 4, cy - r + 9); hCtx.closePath(); hCtx.fill();
      hCtx.restore();
    }

    async function fetchStatus() {
      try {
        const res = await fetch('/api/status');
        const data = await res.json();
        
        const badge = document.getElementById('pcaStatus');
        if (badge) {
          badge.className = data.pcaReady ? 'badge badge-online' : 'badge badge-offline';
          badge.innerText = data.pcaReady ? 'PCA: READY (0x40)' : 'PCA: OFFLINE';
        }
        if (data.extAntenna !== undefined) updateAntennaUI(data.extAntenna, data.rssi);

        if (data.vbat !== undefined) {
          const vbatPill = document.getElementById('vbatPill');
          if (vbatPill) {
            vbatPill.innerText = `${data.vbat.toFixed(1)}V`;
            if (data.vbat < 6.8) vbatPill.style.color = '#ef4444';
            else if (data.vbat < 7.4) vbatPill.style.color = '#f59e0b';
            else vbatPill.style.color = '#00f0ff';
          }
        }

        // IMU Data
        if (data.imu) {
          imuReady = !!data.imu.ready;
          currentPitch = data.imu.pitch || 0.0;
          currentRoll  = data.imu.roll || 0.0;

          const cp = document.getElementById('calibPitch');
          const cr = document.getElementById('calibRoll');
          const cg = document.getElementById('calibGForce');
          if (cp) cp.innerText = `${currentPitch >= 0 ? '+' : ''}${currentPitch.toFixed(1)}°`;
          if (cr) cr.innerText = `${currentRoll >= 0 ? '+' : ''}${currentRoll.toFixed(1)}°`;
          if (cg && data.imu.ax !== undefined) {
            cg.innerText = `ACCEL: ${data.imu.ax.toFixed(2)}g, ${data.imu.ay.toFixed(2)}g, ${data.imu.az.toFixed(2)}g`;
          }
          drawArtificialHorizon('calibHorizonCanvas', currentPitch, currentRoll, imuReady);
        }

        if (data.angles) {
          for (let l = 0; l < 4; l++) {
            for (let j = 0; j < 3; j++) {
              const val = data.angles[l][j];
              const s = document.getElementById(`slider_${l}_${j}`);
              const v = document.getElementById(`val_${l}_${j}`);
              if (s && document.activeElement !== s) s.value = val;
              if (v) v.innerText = `${val}°`;
            }
          }
        }

        if (data.enabled) {
          for (let l = 0; l < 4; l++) {
            for (let j = 0; j < 3; j++) {
              servoPower[l][j] = data.enabled[l][j];
              const b = document.getElementById(`btnPwr_${l}_${j}`);
              const r = document.getElementById(`jointRow_${l}_${j}`);
              if (b) {
                b.className = `switch-btn ${data.enabled[l][j] ? 'on' : 'off'}`;
                b.innerText = data.enabled[l][j] ? 'ON' : 'OFF';
              }
              if (r) {
                if (data.enabled[l][j]) r.classList.remove('joint-disabled');
                else r.classList.add('joint-disabled');
              }
            }
            const anyOn = servoPower[l].some(p => p);
            const lb = document.getElementById(`btnLegPower_${l}`);
            if (lb) {
              lb.className = `switch-btn ${anyOn ? 'on' : 'off'}`;
              lb.innerText = anyOn ? 'PWR ON' : 'PWR OFF';
            }
          }
        }
      } catch (err) {}
    }

    function showToast(msg) {
      const toast = document.getElementById('toast');
      toast.innerText = msg;
      toast.classList.add('show');
      setTimeout(() => toast.classList.remove('show'), 2000);
    }

    buildCards();
    fetchStatus();
    setInterval(fetchStatus, 300);
  </script>
</body>
</html>
)rawliteral";

// =============================================================================
// 3. SETUP & WI-FI CONFIGURATION PAGE HTML (http://arkbot.local/setup)
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

    /* Modal */
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

    #toast { position: fixed; bottom: 24px; left: 50%; transform: translateX(-50%); background: #10b981; color: white; padding: 10px 22px; border-radius: 30px; font-size: 13px; font-weight: 700; box-shadow: 0 8px 24px rgba(0,0,0,0.6); opacity: 0; pointer-events: none; transition: opacity 0.2s ease; z-index: 100; border: 1px solid rgba(255,255,255,0.2); }
    #toast.show { opacity: 1; }
  </style>
</head>
<body>

  <header>
    <div class="brand">
      <img src=")rawliteral" ARK_LOGO_SRC R"rawliteral(" class="brand-logo" alt="ARK-BOT Logo" onclick="location.href='/'">
      <div class="title">
        <h1>ARK-BOT</h1>
        <p>System Setup & Wi-Fi Configuration</p>
      </div>
    </div>

    <!-- Navigation Tabs -->
    <nav class="nav-tabs">
      <a href="/" class="nav-tab">🎮 Commander</a>
      <a href="/calib" class="nav-tab">🎯 Calibrator</a>
      <a href="/setup" class="nav-tab active">⚙️ Setup</a>
      <a href="/update" class="nav-tab">🚀 Update</a>
    </nav>
    
    <div class="header-right">
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

    <!-- Card 3: Battery Voltage & Multiplier Calibration -->
    <div class="card">
      <div class="card-title">
        <span>🔋 Battery Voltage & Multiplier</span>
        <span class="badge badge-online" id="batBadge">MONITORING</span>
      </div>
      <p style="font-size:12px; color:var(--text-dim); margin-bottom:12px;">
        Monitors power supply voltage connected to <strong>Xiao Expansion Pin A0</strong> (10kΩ & 2.2kΩ divider).
      </p>

      <div class="tele-grid" style="margin-bottom:14px;">
        <div class="tele-item" style="border-color:rgba(0,240,255,0.3);">
          <div class="tele-label">Battery Voltage</div>
          <div class="tele-val" id="batVoltageVal" style="color:var(--cyan); font-size:18px; font-weight:800;">-- V</div>
        </div>
        <div class="tele-item">
          <div class="tele-label">Raw Divider (A0)</div>
          <div class="tele-val" id="batDividerVal" style="color:var(--yellow); font-size:16px; font-weight:700;">-- V</div>
        </div>
      </div>

      <form id="batForm" onsubmit="event.preventDefault(); saveBatteryMultiplier();">
        <div class="form-group">
          <label class="form-label" for="batMultInput">Calibration Multiplier</label>
          <div class="input-wrap">
            <input type="number" id="batMultInput" class="input-field" step="0.00001" min="0.01" max="50.0" value="5.06586" required>
          </div>
          <p style="font-size:10px; color:var(--text-dim); margin-top:4px;">
            Default measured ratio: <code>10.0V / 1.974V = 5.06586</code>
          </p>
        </div>

        <div style="background:rgba(0,0,0,0.3); padding:10px; border-radius:8px; border:1px solid var(--card-border); margin-bottom:12px;">
          <div style="font-size:11px; font-weight:700; color:var(--cyan); margin-bottom:6px;">🎯 Quick Multiplier Calculator</div>
          <div style="display:flex; gap:8px; align-items:center;">
            <input type="number" id="actualVoltInput" class="input-field" style="padding:6px 10px; font-size:12px;" step="0.01" placeholder="e.g. 10.00 (Measured V)">
            <button type="button" class="btn btn-dark" style="white-space:nowrap; padding:6px 12px; font-size:11px;" onclick="calculateMultiplierFromActual()">Calculate</button>
          </div>
        </div>

        <button type="submit" class="btn btn-green btn-full" id="btnSaveBat">
          💾 Save Multiplier to NVS
        </button>
      </form>
    </div>

    <!-- Card 4: Network & System Telemetry -->
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

    <!-- Card 5: Device Management -->
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
    </div>
  </div>

  <!-- Reboot Confirmation Modal -->
  <div class="modal-overlay" id="rebootModal">
    <div class="modal-card">
      <div class="modal-title">Restart Robot Controller?</div>
      <div class="modal-body">
        The ESP32-C6 will restart and re-initialize all subsystems.
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
        <span class="footer-title">ARK-BOT SYSTEM &bull; v1.0.6</span>
      </div>
      <div class="footer-credit">
        Designed & Engineered by <span class="author-name">Amuthesan</span>
      </div>
      <div class="footer-org">Ark Technology</div>
    </div>
  </footer>

  <div id="toast">Command Sent</div>

  <script>
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

      networks.sort((a, b) => b.rssi - a.rssi);
      let html = '';
      networks.forEach(net => {
        const isConn = net.connected;
        let signalClass = '#10b981';
        if (net.rssi < -80) signalClass = '#ef4444';
        else if (net.rssi < -68) signalClass = '#f59e0b';
        const authText = net.auth || 'Open';
        const lockIcon = (authText.toLowerCase() === 'open') ? '🔓' : '🔒';

        html += `
          <div class="wifi-item ${isConn ? 'connected' : ''}" onclick="selectNetwork('${escapeHtml(net.ssid)}')">
            <div class="wifi-info">
              <span class="wifi-icon" style="color:${signalClass}">📶</span>
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
      document.getElementById('passInput').focus();
      showToast(`Selected "${ssid}".`);
    }

    function escapeHtml(str) {
      return (str || '').replace(/[&<>"']/g, m => ({
        '&': '&amp;', '<': '&lt;', '>': '&gt;', '"': '&quot;', "'": '&#39;'
      }[m]));
    }

    async function saveWifi() {
      const ssid = document.getElementById('ssidInput').value.trim();
      const pass = document.getElementById('passInput').value;
      if (!ssid) return;

      showToast("Saving credentials and connecting...");
      try {
        const res = await fetch('/api/wifi/save', {
          method: 'POST',
          headers: { 'Content-Type': 'application/x-www-form-urlencoded' },
          body: `ssid=${encodeURIComponent(ssid)}&pass=${encodeURIComponent(pass)}`
        });
        const data = await res.json();
        showToast(data.message || "Saved!");
        setTimeout(fetchWifiStatus, 3000);
      } catch (err) {}
    }

    async function reconnectWifi() {
      showToast("Reconnecting...");
      try {
        await fetch('/api/wifi/reconnect', { method: 'POST' });
        setTimeout(fetchWifiStatus, 3000);
      } catch (err) {}
    }

    async function resetWifi() {
      if (!confirm("Clear saved Wi-Fi credentials and reset to default?")) return;
      try {
        await fetch('/api/wifi/reset', { method: 'POST' });
        showToast("Wi-Fi settings reset to default");
        fetchWifiStatus();
      } catch (err) {}
    }

    async function setAntenna(type) {
      showToast(`Switching to ${type.toUpperCase()} Antenna...`);
      try {
        const res = await fetch(`/api/antenna?type=${type}`, { method: 'POST' });
        const data = await res.json();
        updateAntennaUI(data.extAntenna, data.rssi);
      } catch (err) {}
    }

    function updateAntennaUI(isExt, rssi) {
      const btnInt = document.getElementById('btnAntInt');
      const btnExt = document.getElementById('btnAntExt');
      const rssiBadge = document.getElementById('rssiVal');
      const teleAnt = document.getElementById('teleAnt');
      if (btnInt && btnExt) {
        btnInt.className = isExt ? 'rf-btn' : 'rf-btn active';
        btnExt.className = isExt ? 'rf-btn active' : 'rf-btn';
      }
      if (rssiBadge && rssi !== undefined) rssiBadge.innerText = `📶 ${rssi} dBm`;
      if (teleAnt) teleAnt.innerText = isExt ? 'External (IPEX)' : 'Internal Ceramic';
    }

    function confirmReboot() { document.getElementById('rebootModal').classList.add('show'); }
    function closeRebootModal() { document.getElementById('rebootModal').classList.remove('show'); }

    async function executeReboot() {
      closeRebootModal();
      showToast("ESP32 restarting...");
      try { await fetch('/api/reboot', { method: 'POST' }); } catch (e) {}

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
          badge.className = data.connected ? 'badge badge-online' : 'badge badge-offline';
          badge.innerText = data.connected ? `STA: ${data.staSsid || 'ONLINE'}` : 'MODE: SoftAP ONLY';
        }

        document.getElementById('teleMode').innerText = data.connected ? `STA (${data.staSsid})` : 'SoftAP Fallback';
        document.getElementById('teleStaIp').innerText = data.staIp || 'Not Connected';
        document.getElementById('teleAp').innerHTML = `${data.apSsid}<br>${data.apIp}`;
        document.getElementById('teleMdns').innerText = data.hostname || 'arkbot.local';
        document.getElementById('teleMac').innerText = data.mac || '--';
        document.getElementById('teleRssi').innerText = `${data.rssi || 0} dBm`;

        if (data.extAntenna !== undefined) updateAntennaUI(data.extAntenna, data.rssi);

        if (data.uptime !== undefined) {
          const s = data.uptime;
          const m = Math.floor(s / 60) % 60;
          const h = Math.floor(s / 3600);
          document.getElementById('teleUptime').innerText = `${h}h ${m}m ${s % 60}s`;
        }

        const ssidInput = document.getElementById('ssidInput');
        if (!ssidInput.value && data.staSsid) ssidInput.value = data.staSsid;
      } catch (err) {}
    }

    let currentDividerVoltage = 1.974;

    async function fetchBatteryStatus() {
      try {
        const res = await fetch('/api/status');
        const data = await res.json();
        if (data.vbat !== undefined) {
          const vbatEl = document.getElementById('batVoltageVal');
          const vdivEl = document.getElementById('batDividerVal');
          const multInput = document.getElementById('batMultInput');
          if (vbatEl) {
            vbatEl.innerText = `${data.vbat.toFixed(2)} V`;
            if (data.vbat < 6.8) vbatEl.style.color = 'var(--red)';
            else if (data.vbat < 7.4) vbatEl.style.color = 'var(--yellow)';
            else vbatEl.style.color = 'var(--cyan)';
          }
          if (vdivEl && data.vdiv !== undefined) {
            currentDividerVoltage = data.vdiv;
            vdivEl.innerText = `${data.vdiv.toFixed(3)} V`;
          }
          if (multInput && document.activeElement !== multInput && data.vbat_mult !== undefined) {
            multInput.value = data.vbat_mult.toFixed(5);
          }
        }
      } catch (err) {}
    }

    async function saveBatteryMultiplier() {
      const mult = parseFloat(document.getElementById('batMultInput').value);
      if (isNaN(mult) || mult <= 0) {
        showToast("Invalid multiplier value");
        return;
      }
      showToast("Saving battery multiplier to NVS...");
      try {
        const res = await fetch('/api/battery/multiplier', {
          method: 'POST',
          headers: { 'Content-Type': 'application/x-www-form-urlencoded' },
          body: `mult=${mult}`
        });
        const data = await res.json();
        if (data.status === 'ok') {
          showToast(`Saved! Multiplier: ${mult.toFixed(5)}`);
          fetchBatteryStatus();
        } else {
          showToast(data.message || "Failed to save");
        }
      } catch (err) {
        showToast("Error saving multiplier");
      }
    }

    function calculateMultiplierFromActual() {
      const actual = parseFloat(document.getElementById('actualVoltInput').value);
      if (isNaN(actual) || actual <= 0) {
        showToast("Please enter a valid measured battery voltage");
        return;
      }
      if (currentDividerVoltage <= 0.05) {
        showToast("Divider voltage too low to calculate");
        return;
      }
      const calculatedMult = actual / currentDividerVoltage;
      document.getElementById('batMultInput').value = calculatedMult.toFixed(5);
      showToast(`Calculated: ${calculatedMult.toFixed(5)} (${actual}V / ${currentDividerVoltage.toFixed(3)}V)`);
    }

    function showToast(msg) {
      const toast = document.getElementById('toast');
      toast.innerText = msg;
      toast.classList.add('show');
      setTimeout(() => toast.classList.remove('show'), 2500);
    }

    fetchWifiStatus();
    fetchBatteryStatus();
    setInterval(fetchWifiStatus, 3000);
    setInterval(fetchBatteryStatus, 1500);
    setTimeout(triggerScan, 400);
  </script>
</body>
</html>
)rawliteral";

// =============================================================================
// 4. OTA FIRMWARE UPDATE PAGE HTML (http://arkbot.local/update)
// =============================================================================
const char UPDATE_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no">
  <title>ARK-BOT Firmware Update | Wireless OTA</title>
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

    /* Layout Cards */
    .grid-container { display: grid; grid-template-columns: 1fr; gap: 16px; margin-bottom: 20px; }
    @media(min-width: 800px) { .grid-container { grid-template-columns: 2fr 1fr; } }

    .card { background: var(--card-bg); border: 1px solid var(--card-border); border-radius: 14px; padding: 20px; box-shadow: 0 8px 24px rgba(0,0,0,0.4); }
    .card-header { display: flex; align-items: center; justify-content: space-between; margin-bottom: 16px; }
    .card-title { font-size: 14px; font-weight: 800; color: var(--cyan); text-transform: uppercase; letter-spacing: 0.8px; display: flex; align-items: center; gap: 8px; }
    
    /* Drop Zone */
    .dropzone { border: 2px dashed #0284c7; background: rgba(2, 132, 199, 0.05); border-radius: 12px; padding: 36px 20px; text-align: center; cursor: pointer; transition: all 0.2s ease; position: relative; }
    .dropzone:hover, .dropzone.dragover { border-color: var(--cyan); background: rgba(0, 240, 255, 0.12); transform: scale(1.01); box-shadow: 0 0 20px var(--cyan-glow); }
    .dropzone-icon { font-size: 48px; margin-bottom: 12px; filter: drop-shadow(0 0 8px var(--cyan-glow)); }
    .dropzone-title { font-size: 15px; font-weight: 700; color: #fff; margin-bottom: 6px; }
    .dropzone-sub { font-size: 12px; color: var(--text-dim); }
    .file-input { display: none; }

    /* File Info Box */
    .file-details { display: none; background: #131b2e; border: 1px solid var(--card-border); border-radius: 10px; padding: 14px; margin-top: 16px; }
    .file-row { display: flex; justify-content: space-between; font-size: 13px; margin-bottom: 6px; }
    .file-label { color: var(--text-dim); font-weight: 600; }
    .file-val { color: var(--cyan); font-weight: 700; }

    /* Action Buttons */
    .btn-action { width: 100%; padding: 14px; margin-top: 18px; border-radius: 10px; border: none; font-size: 14px; font-weight: 800; text-transform: uppercase; letter-spacing: 0.8px; cursor: pointer; transition: all 0.2s ease; display: flex; align-items: center; justify-content: center; gap: 10px; }
    .btn-flash { background: linear-gradient(135deg, #0284c7, #00f0ff); color: #000; box-shadow: 0 4px 16px var(--cyan-glow); }
    .btn-flash:hover:not(:disabled) { background: linear-gradient(135deg, #00f0ff, #38bdf8); transform: translateY(-2px); box-shadow: 0 6px 24px var(--cyan-glow); }
    .btn-flash:disabled { background: #1e293b; color: #64748b; cursor: not-allowed; box-shadow: none; }

    /* Progress Bar */
    .progress-box { display: none; margin-top: 20px; }
    .progress-meta { display: flex; justify-content: space-between; font-size: 12px; font-weight: 700; margin-bottom: 8px; color: var(--text-dim); }
    .progress-track { width: 100%; height: 16px; background: #070a12; border-radius: 8px; border: 1px solid var(--card-border); overflow: hidden; position: relative; }
    .progress-fill { height: 100%; width: 0%; background: linear-gradient(90deg, #0284c7, var(--cyan)); border-radius: 8px; transition: width 0.15s linear; box-shadow: 0 0 12px var(--cyan-glow); }

    /* Info Table */
    .info-table { width: 100%; border-collapse: collapse; font-size: 12px; }
    .info-table td { padding: 10px 6px; border-bottom: 1px solid rgba(255,255,255,0.05); }
    .info-table td:first-child { color: var(--text-dim); font-weight: 600; width: 40%; }
    .info-table td:last-child { color: var(--text-main); font-weight: 700; text-align: right; }

    /* Notice Callout */
    .callout { background: rgba(59, 130, 246, 0.08); border-left: 3px solid var(--accent); padding: 12px 14px; border-radius: 0 8px 8px 0; font-size: 11px; line-height: 1.5; color: #cbd5e1; margin-top: 14px; }

    /* Success Modal */
    .modal-overlay { position: fixed; top: 0; left: 0; width: 100%; height: 100%; background: rgba(0,0,0,0.85); backdrop-filter: blur(8px); display: none; align-items: center; justify-content: center; z-index: 999; }
    .modal-overlay.show { display: flex; }
    .modal-card { background: #0f172a; border: 1px solid var(--cyan); box-shadow: 0 0 32px var(--cyan-glow); border-radius: 16px; padding: 28px; width: 90%; max-width: 440px; text-align: center; }
    .modal-icon { font-size: 54px; margin-bottom: 12px; animation: pulse 1.5s infinite; }
    @keyframes pulse { 0% { transform: scale(1); } 50% { transform: scale(1.1); } 100% { transform: scale(1); } }
    .modal-title { font-size: 18px; font-weight: 800; color: #fff; margin-bottom: 8px; }
    .modal-body { font-size: 13px; color: var(--text-dim); line-height: 1.5; margin-bottom: 20px; }

    /* App Footer */
    .app-footer { margin-top: 36px; padding-top: 22px; border-top: 1px solid rgba(255, 255, 255, 0.08); text-align: center; }
    .footer-inner { display: flex; flex-direction: column; align-items: center; gap: 6px; }
    .footer-brand { display: flex; align-items: center; gap: 8px; font-size: 11px; font-weight: 700; color: var(--text-dim); letter-spacing: 1.2px; text-transform: uppercase; }
    .footer-dot { width: 6px; height: 6px; border-radius: 50%; background: var(--cyan); box-shadow: 0 0 8px var(--cyan-glow); }
    .footer-credit { font-size: 13px; color: #cbd5e1; font-weight: 500; }
    .author-name { font-weight: 800; color: #ffffff; background: linear-gradient(90deg, #ffffff, var(--cyan)); -webkit-background-clip: text; -webkit-text-fill-color: transparent; }
    .footer-org { font-size: 11px; color: var(--text-dim); letter-spacing: 0.5px; }

    #toast { position: fixed; bottom: 24px; left: 50%; transform: translateX(-50%); background: #10b981; color: white; padding: 10px 22px; border-radius: 30px; font-size: 13px; font-weight: 700; box-shadow: 0 8px 24px rgba(0,0,0,0.6); opacity: 0; pointer-events: none; transition: opacity 0.2s ease; z-index: 1000; border: 1px solid rgba(255,255,255,0.2); }
    #toast.show { opacity: 1; }
  </style>
</head>
<body>

  <header>
    <div class="brand">
      <img src=")rawliteral" ARK_LOGO_SRC R"rawliteral(" class="brand-logo" alt="ARK-BOT Logo" onclick="location.href='/'">
      <div class="title">
        <h1>ARK-BOT</h1>
        <p>Firmware OTA Manager (v1.0.6)</p>
      </div>
    </div>

    <!-- Navigation Tabs -->
    <nav class="nav-tabs">
      <a href="/" class="nav-tab">🎮 Commander</a>
      <a href="/calib" class="nav-tab">🎯 Calibrator</a>
      <a href="/setup" class="nav-tab">⚙️ Setup</a>
      <a href="/update" class="nav-tab active">🚀 Update</a>
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
      
      <div id="botStatus" class="badge badge-online">STATUS: READY</div>
    </div>
  </header>

  <div class="grid-container">
    <!-- Left Column: OTA Flasher Card -->
    <div class="card">
      <div class="card-header">
        <div class="card-title">
          <span>🚀 Wireless Firmware Flasher</span>
        </div>
        <span style="font-size:11px; font-weight:700; color:var(--text-dim);">ESP32-C6 OTA</span>
      </div>

      <!-- File Dropzone -->
      <div class="dropzone" id="dropzone" onclick="document.getElementById('fileInput').click()">
        <div class="dropzone-icon">📥</div>
        <div class="dropzone-title">Click to Select or Drop Firmware (.bin)</div>
        <div class="dropzone-sub">Accepts compiled PlatformIO firmware binary (firmware.bin)</div>
        <input type="file" id="fileInput" class="file-input" accept=".bin" onchange="handleFileSelect(event)">
      </div>

      <!-- File Details Box -->
      <div class="file-details" id="fileDetails">
        <div class="file-row">
          <span class="file-label">Binary File:</span>
          <span class="file-val" id="fileName">firmware.bin</span>
        </div>
        <div class="file-row">
          <span class="file-label">File Size:</span>
          <span class="file-val" id="fileSize">-- KB</span>
        </div>
        <div class="file-row">
          <span class="file-label">Target Partition:</span>
          <span class="file-val" style="color:#34d399;">OTA App Flash</span>
        </div>
      </div>

      <!-- Progress Box -->
      <div class="progress-box" id="progressBox">
        <div class="progress-meta">
          <span id="progressStatus">Uploading firmware...</span>
          <span id="progressPercent" style="color:var(--cyan);">0%</span>
        </div>
        <div class="progress-track">
          <div class="progress-fill" id="progressFill"></div>
        </div>
      </div>

      <!-- Flash Action Button -->
      <button class="btn-action btn-flash" id="btnFlash" disabled onclick="startOtaUpdate()">
        <span>⚡ START OTA FIRMWARE FLASH</span>
      </button>

      <div class="callout">
        <strong>⚠️ Important Notes:</strong><br>
        &bull; Ensure the robot battery/power supply is stable before flashing.<br>
        &bull; Do not disconnect or refresh this browser window during the upload.<br>
        &bull; ARK-BOT will automatically reboot and reconnect upon successful verification.
      </div>
    </div>

    <!-- Right Column: System Status Card -->
    <div class="card">
      <div class="card-header">
        <div class="card-title">
          <span>⚙️ System Info</span>
        </div>
      </div>

      <table class="info-table">
        <tr>
          <td>Robot Name</td>
          <td>ARK-BOT</td>
        </tr>
        <tr>
          <td>Current Version</td>
          <td style="color:var(--cyan);">v1.0.6</td>
        </tr>
        <tr>
          <td>Hardware MCU</td>
          <td>Seeed XIAO ESP32-C6</td>
        </tr>
        <tr>
          <td>Architecture</td>
          <td>32-bit RISC-V @ 160MHz</td>
        </tr>
        <tr>
          <td>Flash Memory</td>
          <td>4MB Quad-SPI</td>
        </tr>
        <tr>
          <td>OTA Method</td>
          <td>Direct HTTP POST /update</td>
        </tr>
      </table>

      <div style="margin-top:20px;">
        <button class="btn-action" style="background:#1e293b; color:#94a3b8; border:1px solid var(--card-border);" onclick="location.href='/'">
          <span>⬅️ Back to Commander</span>
        </button>
      </div>
    </div>
  </div>

  <!-- Success / Rebooting Modal -->
  <div class="modal-overlay" id="successModal">
    <div class="modal-card">
      <div class="modal-icon">🎉</div>
      <div class="modal-title">Firmware Update Complete!</div>
      <div class="modal-body">
        The new firmware was flashed and verified successfully.<br>
        ARK-BOT is restarting now. Reconnecting in <span id="rebootCount" style="color:var(--cyan); font-weight:800;">10</span>s...
      </div>
      <div class="progress-track" style="height:8px; margin-bottom:12px;">
        <div class="progress-fill" id="modalProgress" style="width:100%;"></div>
      </div>
    </div>
  </div>

  <!-- App Footer -->
  <footer class="app-footer">
    <div class="footer-inner">
      <div class="footer-brand">
        <span class="footer-dot"></span>
        <span class="footer-title">ARK-BOT SYSTEM &bull; v1.0.6</span>
      </div>
      <div class="footer-credit">
        Designed & Engineered by <span class="author-name">Amuthesan</span>
      </div>
      <div class="footer-org">Ark Technology</div>
    </div>
  </footer>

  <div id="toast">Command Sent</div>

  <script>
    let selectedFile = null;

    const dropzone = document.getElementById('dropzone');
    ['dragenter', 'dragover'].forEach(name => {
      dropzone.addEventListener(name, (e) => { e.preventDefault(); dropzone.classList.add('dragover'); }, false);
    });
    ['dragleave', 'drop'].forEach(name => {
      dropzone.addEventListener(name, (e) => { e.preventDefault(); dropzone.classList.remove('dragover'); }, false);
    });
    dropzone.addEventListener('drop', (e) => {
      const dt = e.dataTransfer;
      const files = dt.files;
      if (files.length > 0) processFile(files[0]);
    });

    function handleFileSelect(event) {
      if (event.target.files.length > 0) {
        processFile(event.target.files[0]);
      }
    }

    function processFile(file) {
      if (!file.name.toLowerCase().endsWith('.bin')) {
        showToast("⚠️ Invalid file! Please choose a .bin firmware binary.");
        return;
      }
      selectedFile = file;
      document.getElementById('fileName').innerText = file.name;
      document.getElementById('fileSize').innerText = `${(file.size / 1024).toFixed(1)} KB (${file.size.toLocaleString()} bytes)`;
      document.getElementById('fileDetails').style.display = 'block';
      document.getElementById('btnFlash').disabled = false;
      showToast(`Selected: ${file.name}`);
    }

    function startOtaUpdate() {
      if (!selectedFile) return;

      if (!confirm(`Flash "${selectedFile.name}" (${(selectedFile.size / 1024).toFixed(1)} KB) to ARK-BOT now?`)) {
        return;
      }

      const btnFlash = document.getElementById('btnFlash');
      const progBox = document.getElementById('progressBox');
      const progFill = document.getElementById('progressFill');
      const progPercent = document.getElementById('progressPercent');
      const progStatus = document.getElementById('progressStatus');

      btnFlash.disabled = true;
      btnFlash.innerText = "⚡ FLASHING FIRMWARE...";
      progBox.style.display = 'block';

      const formData = new FormData();
      formData.append("update", selectedFile, selectedFile.name);

      const xhr = new XMLHttpRequest();
      xhr.open("POST", "/update", true);

      xhr.upload.onprogress = function(e) {
        if (e.lengthComputable) {
          const pct = Math.round((e.loaded / e.total) * 100);
          progFill.style.width = pct + '%';
          progPercent.innerText = pct + '%';
          progStatus.innerText = `Uploading: ${(e.loaded / 1024).toFixed(0)} KB / ${(e.total / 1024).toFixed(0)} KB`;
        }
      };

      xhr.onload = function() {
        if (xhr.status === 200) {
          progFill.style.width = '100%';
          progPercent.innerText = '100%';
          progStatus.innerText = "Upload Complete! Verifying & rebooting...";
          showSuccessModal();
        } else {
          showToast("❌ OTA Update Failed! Check binary format.");
          btnFlash.disabled = false;
          btnFlash.innerText = "⚡ START OTA FIRMWARE FLASH";
        }
      };

      xhr.onerror = function() {
        showToast("❌ Network error during firmware upload!");
        btnFlash.disabled = false;
        btnFlash.innerText = "⚡ START OTA FIRMWARE FLASH";
      };

      xhr.send(formData);
    }

    function showSuccessModal() {
      const modal = document.getElementById('successModal');
      modal.classList.add('show');
      let seconds = 10;
      const countElem = document.getElementById('rebootCount');
      const timer = setInterval(() => {
        seconds--;
        if (seconds > 0) {
          countElem.innerText = seconds;
        } else {
          clearInterval(timer);
          location.href = '/';
        }
      }, 1000);
    }

    async function setAntenna(type) {
      showToast(`Switching to ${type.toUpperCase()} Antenna...`);
      try {
        const res = await fetch(`/api/antenna?type=${type}`, { method: 'POST' });
        const data = await res.json();
        updateAntennaUI(data.extAntenna, data.rssi);
      } catch (err) {}
    }

    function updateAntennaUI(isExt, rssi) {
      const btnInt = document.getElementById('btnAntInt');
      const btnExt = document.getElementById('btnAntExt');
      const rssiBadge = document.getElementById('rssiVal');
      if (btnInt && btnExt) {
        btnInt.className = isExt ? 'rf-btn' : 'rf-btn active';
        btnExt.className = isExt ? 'rf-btn active' : 'rf-btn';
      }
      if (rssiBadge && rssi !== undefined) rssiBadge.innerText = `📶 ${rssi} dBm`;
    }

    async function fetchStatus() {
      try {
        const res = await fetch('/api/status');
        const data = await res.json();
        if (data.extAntenna !== undefined) updateAntennaUI(data.extAntenna, data.rssi);
      } catch (err) {}
    }

    function showToast(msg) {
      const toast = document.getElementById('toast');
      toast.innerText = msg;
      toast.classList.add('show');
      setTimeout(() => toast.classList.remove('show'), 2500);
    }

    fetchStatus();
  </script>
</body>
</html>
)rawliteral";
