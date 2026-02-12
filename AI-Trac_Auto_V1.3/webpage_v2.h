#ifndef WEBPAGE_V2_H
#define WEBPAGE_V2_H

const char WEBPAGE_HTML[] PROGMEM = R"rawliteral(

<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Dashboard Traktor Otonom - IoT, GPS, Kamera &amp; ML</title>
    <link rel="preconnect" href="https://fonts.googleapis.com">
    <link rel="preconnect" href="https://fonts.gstatic.com" crossorigin>
    <link href="https://fonts.googleapis.com/css2?family=Inter:wght@400;500;600;700&display=swap" rel="stylesheet">
    <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.4.0/css/all.min.css">
    <link rel="stylesheet" href="https://unpkg.com/leaflet@1.9.4/dist/leaflet.css" />
    <style>
        :root {
            --primary: #0ea5e9;
            --primary-dark: #0284c7;
            --secondary: #06b6d4;
            --success: #10b981;
            --warning: #f59e0b;
            --danger: #ef4444;
            --bg: #0f172a;
            --bg-card: #1e293b;
            --bg-elevated: #334155;
            --text: #f1f5f9;
            --text-muted: #94a3b8;
            --border: rgba(148, 163, 184, 0.2);
            --radius: 12px;
            --radius-sm: 8px;
            --shadow: 0 4px 24px rgba(0,0,0,0.25);
            --font: 'Inter', -apple-system, BlinkMacSystemFont, 'Segoe UI', sans-serif;
            --space-xs: 6px;
            --space-sm: 12px;
            --space-md: 16px;
            --space-lg: 20px;
            --space-xl: 24px;
        }
        .light-theme {
            --bg: #f8fafc;
            --bg-card: #ffffff;
            --bg-elevated: #f1f5f9;
            --text: #0f172a;
            --text-muted: #64748b;
            --border: rgba(0,0,0,0.08);
            --shadow: 0 4px 20px rgba(0,0,0,0.06);
        }

        * { margin: 0; padding: 0; box-sizing: border-box; }
        body {
            font-family: var(--font);
            background: var(--bg);
            color: var(--text);
            min-height: 100vh;
            line-height: 1.5;
            transition: background 0.3s, color 0.3s;
        }

        /* Layout: Sidebar fixed + Main */
        .app-layout {
            display: flex;
            min-height: 100vh;
        }
        .container { max-width: 100%; margin: 0; padding: 0; }

        /* Sidebar - rapi, tap target nyaman */
        .sidebar {
            width: 240px;
            min-width: 240px;
            background: var(--bg-card);
            border-right: 1px solid var(--border);
            display: flex;
            flex-direction: column;
            position: fixed;
            left: 0;
            top: 0;
            bottom: 0;
            z-index: 100;
            transition: transform 0.25s, box-shadow 0.25s;
        }
        .sidebar-header {
            padding: var(--space-lg);
            border-bottom: 1px solid var(--border);
        }
        .sidebar .logo {
            display: flex;
            align-items: center;
            gap: 10px;
        }
        .sidebar .logo-icon {
            width: 38px;
            height: 38px;
            border-radius: var(--radius-sm);
            background: linear-gradient(135deg, var(--primary), var(--secondary));
            display: flex;
            align-items: center;
            justify-content: center;
            color: white;
            font-size: 1.1rem;
        }
        .sidebar .logo h1 { font-size: 1.1rem; font-weight: 700; color: var(--text); }
        .sidebar .subtitle { font-size: 0.65rem; color: var(--text-muted); margin-top: 2px; }
        .sidebar-nav {
            flex: 1;
            overflow-y: auto;
            padding: var(--space-sm) 0;
        }
        .sidebar-nav a {
            display: flex;
            align-items: center;
            gap: var(--space-sm);
            padding: var(--space-md) var(--space-lg);
            min-height: 44px;
            color: var(--text-muted);
            text-decoration: none;
            font-size: 0.9rem;
            font-weight: 500;
            transition: background 0.15s, color 0.15s;
            border-left: 3px solid transparent;
        }
        .sidebar-nav a:hover { background: var(--bg-elevated); color: var(--text); }
        .sidebar-nav a.active { background: rgba(14, 165, 233, 0.1); color: var(--primary); border-left-color: var(--primary); }
        .sidebar-nav a i { width: 22px; text-align: center; opacity: 0.9; font-size: 1rem; }
        .sidebar-footer {
            padding: 12px 16px;
            border-top: 1px solid var(--border);
        }

        .main-wrap {
            flex: 1;
            margin-left: 240px;
            display: flex;
            flex-direction: column;
            min-height: 100vh;
        }

        /* Top Bar - rapi, satu baris kalau muat */
        .top-bar {
            display: flex;
            align-items: center;
            justify-content: space-between;
            flex-wrap: wrap;
            gap: var(--space-sm);
            padding: var(--space-md) var(--space-lg);
            background: var(--bg-card);
            border-bottom: 1px solid var(--border);
        }
        .status-bar {
            display: flex;
            flex-wrap: wrap;
            align-items: center;
            gap: var(--space-xs) var(--space-md);
        }
        .status-item {
            display: flex;
            align-items: center;
            gap: var(--space-xs);
            padding: var(--space-sm) var(--space-md);
            background: var(--bg-elevated);
            border: 1px solid var(--border);
            border-radius: var(--radius-sm);
            font-size: 0.85rem;
        }
        .status-dot {
            width: 8px;
            height: 8px;
            border-radius: 50%;
            background: var(--danger);
            transition: background 0.3s, box-shadow 0.3s;
        }
        .status-dot.online { background: var(--success); box-shadow: 0 0 0 2px rgba(16,185,129,0.4); }
        .status-dot.wifi { background: var(--success); }
        .status-dot.wifi.weak { background: var(--warning); }
        .status-dot.wifi.off { background: var(--danger); }
        .status-label { color: var(--text-muted); }
        .status-value { font-weight: 600; color: var(--primary); }
        .status-item.health-ok .status-dot { background: var(--success); }
        .status-item.health-warn .status-dot { background: var(--warning); }
        .status-item.health-error .status-dot { background: var(--danger); }
        .top-bar-actions { display: flex; align-items: center; gap: 8px; }
        .theme-toggle {
            display: flex;
            align-items: center;
            gap: 8px;
            padding: 8px 14px;
            background: var(--bg-elevated);
            border: 1px solid var(--border);
            border-radius: var(--radius-sm);
            cursor: pointer;
            font-size: 0.85rem;
            color: var(--text);
            transition: background 0.2s;
        }
        .theme-toggle:hover { background: var(--border); }
        .menu-toggle {
            display: none;
            padding: 8px 12px;
            background: var(--bg-elevated);
            border: 1px solid var(--border);
            border-radius: var(--radius-sm);
            color: var(--text);
            cursor: pointer;
            font-size: 1.1rem;
        }

        .main-content {
            flex: 1;
            padding: var(--space-lg);
            overflow: auto;
        }
        .dashboard-grid {
            display: grid;
            grid-template-columns: 1fr 360px;
            grid-template-rows: minmax(400px, 1fr) minmax(300px, auto);
            gap: var(--space-lg);
            max-width: 1600px;
            margin: 0 auto;
        }
        .panel-map { grid-column: 1; grid-row: 1 / 3; min-height: 380px; }
        .panel-control { grid-column: 2; grid-row: 1; display: grid; grid-template-rows: auto auto auto; gap: var(--space-md); align-content: start; }
        .panel-waypoint { grid-column: 1; grid-row: 2; min-height: 260px; }
        .panel-history { grid-column: 2; grid-row: 2; min-height: 260px; }

        .sidebar-overlay {
            display: none;
            position: fixed;
            inset: 0;
            background: rgba(0,0,0,0.4);
            z-index: 99;
            opacity: 0;
            transition: opacity 0.25s;
        }
        .sidebar-overlay.show { display: block; opacity: 1; }
        @media (max-width: 1024px) {
            .sidebar { transform: translateX(-100%); width: 260px; }
            .sidebar.open { transform: translateX(0); box-shadow: 8px 0 24px rgba(0,0,0,0.3); }
            .main-wrap { margin-left: 0; }
            .menu-toggle { display: flex; align-items: center; justify-content: center; min-width: 44px; min-height: 44px; }
            .dashboard-grid {
                grid-template-columns: 1fr;
                grid-template-rows: minmax(320px, auto) auto auto auto;
            }
            .panel-map { grid-column: 1; grid-row: 1; grid-row-span: 1; }
            .panel-control { grid-column: 1; grid-row: 2; }
            .panel-waypoint { grid-column: 1; grid-row: 3; }
            .panel-history { grid-column: 1; grid-row: 4; }
        }

        .map-section {
            background: var(--bg-card);
            border-radius: var(--radius);
            border: 1px solid var(--border);
            box-shadow: var(--shadow);
            overflow: hidden;
            display: flex;
            flex-direction: column;
            min-height: 340px;
            height: 100%;
        }
        .map-header {
            display: flex;
            justify-content: space-between;
            align-items: center;
            flex-wrap: wrap;
            gap: var(--space-sm);
            padding: var(--space-md) var(--space-lg);
            border-bottom: 1px solid var(--border);
        }
        .map-header h2 {
            font-size: 1.15rem;
            font-weight: 600;
            color: var(--text);
            display: flex;
            align-items: center;
            gap: var(--space-sm);
        }
        .map-header h2 i { color: var(--primary); }
        .map-controls {
            display: flex;
            gap: var(--space-xs);
            flex-wrap: wrap;
            align-items: center;
        }
        .map-controls .btn { min-height: 36px; padding: 8px 12px; }
        .map-controls-group { display: flex; flex-wrap: wrap; gap: var(--space-xs); align-items: center; }
        .map-container {
            flex: 1;
            min-height: 260px;
            position: relative;
            overflow: hidden;
        }

        #onlineMap { width: 100%; height: 100%; z-index: 1; }
        #offlineGrid { width: 100%; height: 100%; position: relative; background: var(--bg-elevated); display: none; }
        .grid-line { position: absolute; background: rgba(16, 185, 129, 0.25); }
        .grid-line.horizontal { width: 100%; height: 1px; left: 0; }
        .grid-line.vertical { height: 100%; width: 1px; top: 0; }
        .grid-coordinates { position: absolute; color: var(--text-muted); font-size: 0.7rem; pointer-events: none; }
        .vehicle-marker { position: absolute; width: 20px; height: 20px; background: var(--danger); border-radius: 50%; border: 2px solid white; transform: translate(-50%, -50%); z-index: 10; }
        .vehicle-marker::after { content: ''; position: absolute; top: 50%; left: 50%; width: 24px; height: 2px; background: white; transform: translate(-50%, -50%) rotate(var(--heading, 0deg)); transform-origin: center; }
        .waypoint-marker { position: absolute; width: 14px; height: 14px; background: var(--warning); border-radius: 50%; border: 2px solid white; transform: translate(-50%, -50%); cursor: pointer; z-index: 5; }
        .map-overlay {
            position: absolute; top: 10px; left: 10px;
            background: rgba(15, 23, 42, 0.85); color: white;
            padding: 6px 12px; border-radius: var(--radius-sm); font-size: 0.75rem; z-index: 100;
            display: flex; align-items: center; gap: 8px;
        }

        .control-section { display: flex; flex-direction: column; gap: var(--space-md); }
        .card {
            background: var(--bg-card);
            border-radius: var(--radius);
            border: 1px solid var(--border);
            box-shadow: var(--shadow);
            padding: var(--space-lg);
            overflow: hidden;
            min-height: 0;
        }
        .panel-waypoint.card {
            height: 100%;
            display: flex;
            flex-direction: column;
            min-height: 0;
        }
        .panel-waypoint .waypoint-list { flex: 1; min-height: 0; }
        .card h3 {
            font-size: 1rem;
            font-weight: 600;
            color: var(--text);
            margin-bottom: var(--space-md);
            padding-bottom: var(--space-sm);
            border-bottom: 1px solid var(--border);
            display: flex;
            align-items: center;
            gap: var(--space-sm);
        }
        .card h3 i { color: var(--primary); font-size: 1.05rem; }
        .info-grid { display: grid; grid-template-columns: 1fr 1fr; gap: 10px 14px; }
        .info-item { display: flex; flex-direction: column; gap: 2px; }
        .info-label { font-size: 0.7rem; color: var(--text-muted); text-transform: uppercase; letter-spacing: 0.03em; }
        .info-value { font-weight: 600; font-size: 0.85rem; color: var(--text); display: flex; align-items: center; gap: 6px; }
        .signal-indicator { display: flex; align-items: center; gap: 2px; }
        .signal-bar { width: 4px; height: 10px; background: var(--bg-elevated); border-radius: 2px; }
        .signal-bar.active { background: var(--success); }
        .signal-bar.active.medium { background: var(--warning); }
        .signal-bar.active.weak { background: var(--danger); }
        .button-group { display: flex; gap: 8px; flex-wrap: wrap; margin-top: 10px; }
        .btn {
            padding: var(--space-sm) var(--space-md);
            min-height: 38px;
            border: none;
            border-radius: var(--radius-sm);
            font-weight: 600;
            font-size: 0.85rem;
            cursor: pointer;
            display: inline-flex;
            align-items: center;
            justify-content: center;
            gap: var(--space-xs);
            transition: background 0.2s, transform 0.15s, box-shadow 0.2s;
        }
        .btn:hover { transform: translateY(-1px); }
        .btn:active { transform: translateY(0); }
        .btn:disabled { opacity: 0.5; cursor: not-allowed; transform: none; }
        .btn:focus-visible { outline: 2px solid var(--primary); outline-offset: 2px; }
        .btn-primary { background: linear-gradient(135deg, var(--primary), var(--primary-dark)); color: white; }
        .btn-primary:hover { opacity: 0.95; }
        .btn-secondary { background: var(--bg-elevated); color: var(--text); border: 1px solid var(--border); }
        .btn-secondary:hover { background: var(--border); }
        .btn-success { background: var(--success); color: white; }
        .btn-danger { background: var(--danger); color: white; }
        .btn-warning { background: var(--warning); color: white; }
        .btn-sm { padding: 6px 10px; font-size: 0.75rem; }
        .btn-emergency {
            background: var(--danger);
            color: white;
            border: 2px solid rgba(239, 68, 68, 0.8);
            font-weight: 700;
            padding: 12px 16px;
            animation: emergency-pulse 2s ease-in-out infinite;
        }
        .btn-emergency:hover { background: #dc2626; border-color: var(--danger); }
        @keyframes emergency-pulse {
            0%, 100% { box-shadow: 0 0 0 0 rgba(239, 68, 68, 0.4); }
            50% { box-shadow: 0 0 0 8px rgba(239, 68, 68, 0); }
        }

        .time-display { font-size: 1.5rem; font-weight: 700; text-align: center; margin: 6px 0; color: var(--primary); letter-spacing: 0.02em; }
        .time-source { font-size: 0.75rem; color: var(--text-muted); text-align: center; margin-bottom: 8px; }
        .time-selector { margin-top: 10px; }
        .time-zone-select { padding: 8px 10px; border-radius: var(--radius-sm); background: var(--bg-elevated); border: 1px solid var(--border); color: var(--text); font-size: 0.85rem; width: 100%; }
        .mode-selector { display: grid; grid-template-columns: repeat(3, 1fr); gap: 8px; margin-bottom: 12px; }
        .mode-btn {
            padding: 10px 6px;
            border: 1px solid var(--border);
            border-radius: var(--radius-sm);
            background: var(--bg-elevated);
            color: var(--text);
            font-weight: 600;
            font-size: 0.8rem;
            cursor: pointer;
            text-align: center;
            transition: all 0.2s;
        }
        .mode-btn:hover { border-color: var(--primary); color: var(--primary); }
        .mode-btn.active { background: var(--primary); border-color: var(--primary); color: white; }
        .control-stats { display: grid; grid-template-columns: 1fr 1fr; gap: 8px; margin-top: 12px; }
        .stat-item {
            padding: 10px;
            background: var(--bg-elevated);
            border-radius: var(--radius-sm);
            text-align: center;
            border: 1px solid var(--border);
        }
        .stat-label { font-size: 0.7rem; color: var(--text-muted); margin-bottom: 4px; }
        .stat-value { font-size: 1.2rem; font-weight: 700; color: var(--primary); }
        .stat-unit { font-size: 0.7rem; color: var(--text-muted); margin-left: 2px; }
        .control-values { display: grid; grid-template-columns: 1fr 1fr; gap: 8px; margin-top: 10px; }
        .control-value { padding: 10px; background: var(--bg-elevated); border-radius: var(--radius-sm); text-align: center; border: 1px solid var(--border); }
        .control-label { font-size: 0.7rem; color: var(--text-muted); margin-bottom: 2px; }
        .control-number { font-size: 1.35rem; font-weight: 700; color: var(--primary); }
        .navigation-controls { display: grid; grid-template-columns: 1fr 1fr; gap: 8px; margin-top: 12px; }
        .navigation-controls .btn-emergency { grid-column: 1 / -1; }
        .history-filters { display: flex; gap: 8px; flex-wrap: wrap; margin-bottom: 10px; }
        .history-filters select {
            padding: 6px 10px;
            border-radius: var(--radius-sm);
            border: 1px solid var(--border);
            background: var(--bg-elevated);
            color: var(--text);
            font-size: 0.8rem;
        }
        .wifi-scan-container { margin-top: 12px; }
        .wifi-list {
            max-height: 180px;
            overflow-y: auto;
            border-radius: var(--radius-sm);
            border: 1px solid var(--border);
            margin-top: 8px;
            background: var(--bg-elevated);
        }
        .wifi-item {
            display: flex;
            justify-content: space-between;
            align-items: center;
            padding: 10px 12px;
            border-bottom: 1px solid var(--border);
            cursor: pointer;
            transition: background 0.15s;
        }
        .wifi-item:last-child { border-bottom: none; }
        .wifi-item:hover { background: var(--bg-card); }
        .wifi-item.connected { border-left: 3px solid var(--success); background: rgba(16, 185, 129, 0.08); }
        .wifi-info { flex: 1; min-width: 0; }
        .wifi-ssid { font-weight: 600; font-size: 0.8rem; display: flex; align-items: center; gap: 6px; flex-wrap: wrap; }
        .wifi-encryption { font-size: 0.65rem; padding: 2px 6px; border-radius: 4px; background: var(--warning); color: white; }
        .wifi-open { background: var(--secondary); }
        .wifi-channel { font-size: 0.7rem; color: var(--text-muted); margin-left: 6px; }
        .wifi-signal { font-size: 0.7rem; color: var(--text-muted); margin-top: 2px; display: flex; align-items: center; gap: 4px; }
        .signal-strength { font-weight: 600; }
        .signal-strength.excellent { color: var(--success); }
        .signal-strength.good { color: var(--warning); }
        .signal-strength.fair, .signal-strength.weak { color: var(--danger); }
        .wifi-actions { display: flex; gap: 6px; flex-shrink: 0; }
        .wifi-connect-btn { padding: 5px 10px; font-size: 0.75rem; border-radius: var(--radius-sm); white-space: nowrap; }

        .usage-log {
            background: var(--bg-card);
            border-radius: var(--radius);
            border: 1px solid var(--border);
            box-shadow: var(--shadow);
            padding: 18px;
            height: 100%;
            display: flex;
            flex-direction: column;
            min-height: 0;
            overflow: hidden;
        }
        .usage-log h3 { font-size: 0.95rem; font-weight: 600; color: var(--text); margin-bottom: 12px; padding-bottom: 10px; border-bottom: 1px solid var(--border); display: flex; align-items: center; gap: 8px; }
        .usage-log h3 i { color: var(--primary); }
        .log-controls { display: flex; justify-content: space-between; align-items: center; flex-wrap: wrap; gap: 8px; margin-bottom: 12px; }
        .log-table-container { overflow: auto; max-height: 260px; border-radius: var(--radius-sm); border: 1px solid var(--border); }
        .log-table { width: 100%; border-collapse: collapse; font-size: 0.78rem; }
        .log-table th {
            background: var(--bg-elevated);
            color: var(--text-muted);
            font-weight: 600;
            padding: 10px 12px;
            text-align: left;
            position: sticky;
            top: 0;
            z-index: 1;
        }
        .log-table td { padding: 8px 12px; border-bottom: 1px solid var(--border); }
        .log-table tr:hover { background: var(--bg-elevated); }
        .log-timestamp { color: var(--text-muted); white-space: nowrap; font-size: 0.75rem; }
        .log-mode { font-weight: 600; padding: 3px 8px; border-radius: 6px; font-size: 0.7rem; }
        .log-mode.manual { background: rgba(14, 165, 233, 0.15); color: var(--primary); }
        .log-mode.auto { background: rgba(16, 185, 129, 0.15); color: var(--success); }
        .log-mode.line { background: rgba(245, 158, 11, 0.15); color: var(--warning); }
        .log-distance { text-align: right; font-weight: 600; }
        .log-battery { text-align: center; }
        .battery-indicator { display: inline-flex; align-items: center; gap: 6px; }
        .battery-level { width: 36px; height: 14px; border: 1px solid var(--border); border-radius: 4px; overflow: hidden; }
        .battery-fill { height: 100%; transition: width 0.3s; }
        .battery-percentage { font-size: 0.75rem; font-weight: 600; }
        .battery-low { background: var(--danger); }
        .battery-medium { background: var(--warning); }
        .battery-high { background: var(--success); }
        .footer-info {
            margin-top: 24px;
            padding-top: 16px;
            border-top: 1px solid var(--border);
            display: flex;
            justify-content: space-between;
            font-size: 0.78rem;
            color: var(--text-muted);
        }
        .toast {
            position: fixed;
            bottom: 20px;
            right: 20px;
            background: var(--bg-card);
            color: var(--text);
            padding: 12px 18px;
            border-radius: var(--radius-sm);
            border: 1px solid var(--border);
            box-shadow: var(--shadow);
            display: flex;
            align-items: center;
            gap: 10px;
            z-index: 1000;
            transform: translateY(120%);
            opacity: 0;
            transition: transform 0.3s, opacity 0.3s;
            font-size: 0.9rem;
            max-width: 320px;
        }
        .toast.show { transform: translateY(0); opacity: 1; }
        .toast.error { border-left: 4px solid var(--danger); }
        .toast.warning { border-left: 4px solid var(--warning); }
        .toast:not(.error):not(.warning) { border-left: 4px solid var(--success); }
        .gps-status { display: flex; align-items: center; gap: 6px; margin-bottom: 8px; }
        .gps-status-indicator { width: 8px; height: 8px; border-radius: 50%; background: var(--danger); }
        .gps-status-indicator.active { background: var(--success); animation: pulse 2s infinite; }
        @keyframes pulse { 0%,100% { opacity: 1; } 50% { opacity: 0.5; } }
        .password-modal {
            position: fixed; inset: 0;
            background: rgba(0,0,0,0.5);
            display: flex;
            justify-content: center;
            align-items: center;
            z-index: 10000;
            opacity: 0;
            visibility: hidden;
            transition: opacity 0.25s, visibility 0.25s;
        }
        .password-modal.show { opacity: 1; visibility: visible; }
        .password-content {
            background: var(--bg-card);
            padding: 24px;
            border-radius: var(--radius);
            width: 90%;
            max-width: 380px;
            border: 1px solid var(--border);
            box-shadow: var(--shadow);
        }
        .password-content h3 { margin-bottom: 16px; font-size: 1rem; color: var(--text); }
        .password-input {
            width: 100%;
            padding: 12px 14px;
            margin-bottom: 16px;
            border: 1px solid var(--border);
            border-radius: var(--radius-sm);
            background: var(--bg-elevated);
            color: var(--text);
            font-size: 0.9rem;
        }
        .password-buttons { display: flex; gap: 10px; justify-content: flex-end; }
        .waypoint-list { max-height: 200px; overflow-y: auto; list-style: none; padding: 0; margin: 0; border-radius: var(--radius-sm); border: 1px solid var(--border); }
        .waypoint-list li {
            display: flex;
            align-items: center;
            justify-content: space-between;
            padding: 10px 12px;
            border-bottom: 1px solid var(--border);
            font-size: 0.82rem;
        }
        .waypoint-list li:last-child { border-bottom: none; }
        .waypoint-num { font-weight: 700; color: var(--primary); min-width: 88px; }
        .waypoint-coords { font-family: ui-monospace, monospace; font-size: 0.72rem; color: var(--text-muted); }
        .history-tabs { display: flex; gap: 4px; margin-bottom: 12px; }
        .history-tab {
            padding: 8px 14px;
            border: 1px solid var(--border);
            border-radius: var(--radius-sm);
            background: var(--bg-elevated);
            color: var(--text);
            cursor: pointer;
            font-size: 0.82rem;
            font-weight: 500;
        }
        .history-tab:hover { border-color: var(--primary); color: var(--primary); }
        .history-tab.active { background: var(--primary); border-color: var(--primary); color: white; }
        .history-view { display: none; min-height: 180px; }
        .history-view.active { display: block; }
        .replay-controls { display: flex; align-items: center; gap: 10px; margin-top: 10px; }
        .replay-slider { flex: 1; height: 6px; accent-color: var(--primary); }
        .chart-container { height: 180px; position: relative; border-radius: var(--radius-sm); overflow: hidden; }

        /* Dashboard Overview - grid cards, no long scroll */
        .dashboard-overview { display: grid; grid-template-columns: repeat(4, 1fr); grid-template-rows: auto auto; gap: var(--space-md); margin-bottom: var(--space-xl); }
        .overview-card { background: var(--bg-card); border: 1px solid var(--border); border-radius: var(--radius); padding: var(--space-md); box-shadow: var(--shadow); }
        .overview-card h4 { font-size: 0.75rem; color: var(--text-muted); text-transform: uppercase; letter-spacing: 0.04em; margin-bottom: var(--space-xs); }
        .overview-card .value { font-size: 1.1rem; font-weight: 700; color: var(--primary); line-height: 1.3; }
        .overview-card.wide { grid-column: span 2; }
        .overview-card.tall { grid-row: span 2; min-height: 200px; }
        .mini-map-preview { width: 100%; height: 140px; background: var(--bg-elevated); border-radius: var(--radius-sm); overflow: hidden; position: relative; }
        .mini-map-preview .hint { position: absolute; inset: 0; display: flex; align-items: center; justify-content: center; color: var(--text-muted); font-size: 0.8rem; }
        .quick-control-overview { display: flex; gap: var(--space-sm); flex-wrap: wrap; margin-top: var(--space-sm); }
        .section-hint { font-size: 0.8rem; color: var(--text-muted); margin-top: var(--space-xs); line-height: 1.4; }
        .monitoring-snapshot { width: 100%; height: 120px; background: var(--bg-elevated); border-radius: var(--radius-sm); overflow: hidden; position: relative; }
        .monitoring-snapshot .camera-placeholder { width: 100%; height: 100%; background: linear-gradient(135deg, var(--bg-elevated) 0%, var(--border) 100%); display: flex; align-items: center; justify-content: center; color: var(--text-muted); font-size: 0.75rem; }
        .monitoring-snapshot .ml-badge { position: absolute; bottom: 6px; right: 6px; background: rgba(0,0,0,0.7); color: var(--success); padding: 4px 8px; border-radius: 4px; font-size: 0.7rem; font-weight: 600; }
        .summary-stats { display: grid; grid-template-columns: repeat(3, 1fr); gap: 8px; margin-top: 10px; }
        .summary-stats .stat { text-align: center; padding: 8px; background: var(--bg-elevated); border-radius: var(--radius-sm); font-size: 0.8rem; }
        .summary-stats .stat .num { font-weight: 700; color: var(--primary); }
        @media (max-width: 900px) { .dashboard-overview { grid-template-columns: 1fr 1fr; } .overview-card.wide { grid-column: span 2; } }
        @media (max-width: 600px) { .dashboard-overview { grid-template-columns: 1fr; } .overview-card.wide { grid-column: span 1; } .summary-stats { grid-template-columns: 1fr; } }

        /* Monitoring Lahan - Kamera + ML */
        .monitoring-lahan { display: grid; grid-template-columns: 1fr 280px; gap: var(--space-lg); margin-bottom: var(--space-xl); }
        .camera-feed-container { background: var(--bg-card); border: 1px solid var(--border); border-radius: var(--radius); overflow: hidden; position: relative; min-height: 320px; }
        .camera-feed { width: 100%; height: 100%; min-height: 300px; background: var(--bg-elevated); display: flex; align-items: center; justify-content: center; color: var(--text-muted); font-size: 0.9rem; }
        .camera-feed video, .camera-feed img { max-width: 100%; max-height: 100%; object-fit: contain; }
        .ml-overlay { position: absolute; inset: 0; pointer-events: none; }
        .ml-bounding-box { position: absolute; border: 2px solid; border-radius: 2px; font-size: 0.65rem; font-weight: 600; padding: 2px 4px; white-space: nowrap; }
        .ml-bounding-box.plant { border-color: var(--success); color: var(--success); }
        .ml-bounding-box.weed { border-color: var(--warning); color: var(--warning); }
        .ml-bounding-box.obstacle { border-color: var(--danger); color: var(--danger); }
        .ml-bounding-box.water { border-color: var(--secondary); color: var(--secondary); }
        .insight-monitoring { display: flex; flex-direction: column; gap: var(--space-md); }
        .insight-card { padding: var(--space-md); background: var(--bg-elevated); border-radius: var(--radius-sm); border: 1px solid var(--border); }
        .insight-card h4 { font-size: 0.8rem; margin-bottom: 8px; color: var(--text); }
        .insight-card .obj-list { font-size: 0.75rem; color: var(--text-muted); }
        .insight-card .status-lahan { font-weight: 600; margin-top: 8px; padding: 6px 10px; border-radius: 6px; }
        .insight-card .status-lahan.normal { background: rgba(16,185,129,0.2); color: var(--success); }
        .insight-card .status-lahan.bermasalah { background: rgba(239,68,68,0.2); color: var(--danger); }
        @media (max-width: 768px) { .monitoring-lahan { grid-template-columns: 1fr; } }

        /* Settings page - tabs */
        .settings-panel { max-width: 800px; margin: 0 auto; }
        .settings-tabs { display: flex; gap: 4px; flex-wrap: wrap; margin-bottom: 16px; border-bottom: 1px solid var(--border); padding-bottom: 12px; }
        .settings-tab { padding: 8px 14px; border: 1px solid var(--border); border-radius: var(--radius-sm); background: var(--bg-elevated); color: var(--text-muted); cursor: pointer; font-size: 0.85rem; }
        .settings-tab:hover { color: var(--primary); border-color: var(--primary); }
        .settings-tab.active { background: var(--primary); border-color: var(--primary); color: white; }
        .settings-section { display: none; padding: 16px; background: var(--bg-card); border: 1px solid var(--border); border-radius: var(--radius); }
        .settings-section.active { display: block; }
        .settings-section h3 { font-size: 1rem; margin-bottom: 12px; color: var(--text); }
        .settings-row { display: flex; justify-content: space-between; align-items: center; padding: 10px 0; border-bottom: 1px solid var(--border); flex-wrap: wrap; gap: 8px; }
        .settings-row:last-child { border-bottom: none; }
        .settings-row label { font-size: 0.9rem; color: var(--text); }
        .settings-row input, .settings-row select { padding: 6px 10px; border-radius: var(--radius-sm); border: 1px solid var(--border); background: var(--bg-elevated); color: var(--text); font-size: 0.85rem; }

        /* Virtual Joystick - Manual Mode */
        .joystick-container { width: 120px; height: 120px; margin: 10px auto; background: var(--bg-elevated); border: 2px solid var(--border); border-radius: 50%; position: relative; touch-action: none; }
        .joystick-stick { width: 40px; height: 40px; background: var(--primary); border-radius: 50%; position: absolute; left: 50%; top: 50%; transform: translate(-50%, -50%); cursor: grab; box-shadow: 0 2px 8px rgba(0,0,0,0.3); transition: box-shadow 0.15s; }
        .joystick-stick:active { cursor: grabbing; box-shadow: 0 4px 12px rgba(14,165,233,0.5); }
        .joystick-zone { display: none; }
        .waypoint-validation { margin-top: 8px; padding: 8px 12px; border-radius: var(--radius-sm); font-size: 0.8rem; }
        .waypoint-validation.valid { background: rgba(16,185,129,0.15); color: var(--success); border: 1px solid var(--success); }
        .waypoint-validation.invalid { background: rgba(239,68,68,0.15); color: var(--danger); border: 1px solid var(--danger); }
        .waypoint-validation.warning { background: rgba(245,158,11,0.15); color: var(--warning); border: 1px solid var(--warning); }
        .skeleton { background: linear-gradient(90deg, var(--bg-elevated) 25%, var(--border) 50%, var(--bg-elevated) 75%); background-size: 200% 100%; animation: skeleton 1.5s ease-in-out infinite; border-radius: 4px; }
        @keyframes skeleton { 0% { background-position: 200% 0; } 100% { background-position: -200% 0; } }

        @media (max-width: 768px) {
            .main-content { padding: var(--space-md); }
            .dashboard-grid { gap: var(--space-md); }
            .card { padding: var(--space-md); }
            .info-grid { grid-template-columns: 1fr; }
            .control-stats, .control-values, .navigation-controls, .mode-selector { grid-template-columns: 1fr; }
            .map-controls { flex-wrap: wrap; gap: var(--space-sm); }
            .btn { min-height: 44px; padding: var(--space-sm) var(--space-md); font-size: 0.9rem; }
            .status-bar { width: 100%; }
            .top-bar { padding: var(--space-sm) var(--space-md); }
        }
        @media (max-width: 480px) {
            .logo h1 { font-size: 1.1rem; }
            .overview-card .value { font-size: 1rem; }
        }
    </style>
</head>
<body>
    <div class="app-layout">
        <div class="sidebar-overlay" id="sidebarOverlay" aria-hidden="true"></div>
        <aside class="sidebar" id="sidebar">
            <div class="sidebar-header">
                <div class="logo">
                    <div class="logo-icon"><i class="fas fa-tractor"></i></div>
                    <div>
                        <h1>Traktor Otonom</h1>
                        <p class="subtitle">IoT, GPS, Kamera &amp; ML</p>
                    </div>
                </div>
            </div>
            <nav class="sidebar-nav">
                <a href="#dashboard" class="nav-link active" data-section="dashboard"><i class="fas fa-th-large"></i> Dashboard</a>
                <a href="#live-map" class="nav-link" data-section="live-map"><i class="fas fa-map-marked-alt"></i> Live Map</a>
                <a href="#waypoint-manager" class="nav-link" data-section="waypoint-manager"><i class="fas fa-map-pin"></i> Waypoint Manager</a>
                <a href="#control-mode" class="nav-link" data-section="control-mode"><i class="fas fa-gamepad"></i> Control Mode</a>
                <a href="#monitoring-lahan" class="nav-link" data-section="monitoring-lahan"><i class="fas fa-leaf"></i> Monitoring Lahan</a>
                <a href="#history-analytics" class="nav-link" data-section="history-analytics"><i class="fas fa-chart-line"></i> History &amp; Analytics</a>
                <a href="#network-system" class="nav-link" data-section="network-system"><i class="fas fa-wifi"></i> Network &amp; System</a>
                <a href="#settings" class="nav-link" data-section="settings"><i class="fas fa-cog"></i> Settings</a>
            </nav>
            <div class="sidebar-footer">
                <p style="font-size:0.75rem;color:var(--text-muted);">Modern Industrial Smart Agriculture</p>
            </div>
        </aside>

        <div class="main-wrap">
            <header class="top-bar">
                <button class="menu-toggle" id="menuToggle" aria-label="Menu"><i class="fas fa-bars"></i></button>
                <div class="status-bar">
                    <div class="status-item" title="Status GPS">
                        <span class="status-dot" id="statusGps"></span>
                        <span class="status-label">GPS</span>
                        <span class="status-value" id="statusGpsVal">--</span>
                    </div>
                    <div class="status-item" title="Koneksi WiFi">
                        <span class="status-dot wifi" id="statusWifi"></span>
                        <span class="status-label">WiFi</span>
                        <span class="status-value" id="statusWifiVal">--</span>
                    </div>
                    <div class="status-item" title="Mode operasi">
                        <span class="status-dot" id="statusMode"></span>
                        <span class="status-value" id="statusModeVal">MANUAL</span>
                    </div>
                    <div class="status-item" title="Waktu GPS">
                        <i class="fas fa-clock"></i>
                        <span class="status-value" id="statusTime">--:--:--</span>
                    </div>
                    <div class="status-item" title="Daya sistem">
                        <i class="fas fa-battery-half" style="color: var(--success);"></i>
                        <span class="status-value" id="statusBattery">--%</span>
                    </div>
                    <div class="status-item health-ok" id="statusHealthItem" title="System Health">
                        <span class="status-dot" id="statusHealth"></span>
                        <span class="status-label">Health</span>
                        <span class="status-value" id="statusHealthVal">OK</span>
                    </div>
                </div>
                <div class="top-bar-actions">
                    <div class="theme-toggle" id="themeToggle"><i class="fas fa-sun"></i><span>Terang</span></div>
                </div>
            </header>

            <main class="main-content" id="dashboard">
                <p class="section-hint" style="margin-bottom:var(--space-md);">Ringkasan status traktor dan kontrol cepat. Gunakan menu samping untuk peta, waypoint, dan pengaturan.</p>
                <!-- Dashboard Overview - ringkasan sistem, bukan peta -->
                <section id="dashboard-overview" class="dashboard-overview">
                    <div class="overview-card">
                        <h4>Posisi Traktor</h4>
                        <div class="value" id="overviewPosition">--, --</div>
                        <p style="font-size:0.7rem;color:var(--text-muted);margin-top:4px;">Lat, Lon</p>
                    </div>
                    <div class="overview-card">
                        <h4>Mode Aktif</h4>
                        <div class="value" id="overviewMode">MANUAL</div>
                    </div>
                    <div class="overview-card">
                        <h4>Koneksi</h4>
                        <div class="value" id="overviewConnection">GPS / WiFi</div>
                    </div>
                    <div class="overview-card">
                        <h4>Kamera &amp; ML</h4>
                        <div class="value" id="overviewML">0 objek</div>
                    </div>
                    <div class="overview-card wide tall">
                        <h4>Mini Map Preview</h4>
                        <div class="mini-map-preview" id="miniMapPreview">
                            <span class="hint">Klik Live Map untuk peta penuh</span>
                        </div>
                    </div>
                    <div class="overview-card">
                        <h4>Quick Control</h4>
                        <div class="quick-control-overview">
                            <button class="btn btn-success btn-sm" id="overviewStart"><i class="fas fa-play"></i> Start</button>
                            <button class="btn btn-secondary btn-sm" id="overviewStop"><i class="fas fa-stop"></i> Stop</button>
                            <button class="btn btn-emergency btn-sm" id="overviewEmergency"><i class="fas fa-exclamation-triangle"></i> Emergency</button>
                        </div>
                    </div>
                    <div class="overview-card">
                        <h4>Monitoring Snapshot</h4>
                        <div class="monitoring-snapshot">
                            <div class="camera-placeholder"><i class="fas fa-video"></i> Kamera</div>
                            <span class="ml-badge" id="overviewMLCount">0 objek</span>
                        </div>
                    </div>
                    <div class="overview-card">
                        <h4>Summary Hari Ini</h4>
                        <div class="summary-stats">
                            <div class="stat"><span class="num" id="overviewHours">0</span> jam</div>
                            <div class="stat"><span class="num" id="overviewArea">0</span> m²</div>
                            <div class="stat"><span class="num" id="overviewErrors">0</span> error</div>
                        </div>
                    </div>
                </section>

                <div class="dashboard-grid">
            <section id="live-map" class="map-section panel-map">
                <div class="map-header">
                    <h2><i class="fas fa-map-marked-alt"></i> Live Map</h2>
                    <div class="map-controls">
                        <span class="map-controls-group">
                            <button class="btn btn-secondary" id="mapLayerStreet" title="Tampilkan peta jalan" aria-label="Layer peta jalan">Street</button>
                            <button class="btn btn-secondary" id="mapLayerSatellite" title="Tampilkan peta satelit" aria-label="Layer satelit">Satelit</button>
                            <button class="btn btn-secondary" id="centerMap" title="Pusatkan ke posisi traktor" aria-label="Pusatkan peta"><i class="fas fa-crosshairs"></i> Center</button>
                            <button class="btn btn-warning" id="toggleMapMode" title="Ganti mode online/offline" aria-label="Mode peta"><i class="fas fa-sync-alt"></i> Offline</button>
                        </span>
                        <span class="map-controls-group">
                            <button class="btn btn-secondary" id="clearWaypoints" title="Hapus semua waypoint" aria-label="Hapus waypoint"><i class="fas fa-trash-alt"></i> Hapus WP</button>
                            <button class="btn btn-success" id="startNavigation" title="Mulai navigasi otomatis" aria-label="Mulai"><i class="fas fa-play"></i> Start</button>
                            <button class="btn btn-warning" id="pauseNavigation" title="Jeda navigasi" aria-label="Jeda"><i class="fas fa-pause"></i> Pause</button>
                            <button class="btn btn-secondary" id="stopNavigation" title="Hentikan navigasi" aria-label="Stop"><i class="fas fa-stop"></i> Stop</button>
                        </span>
                    </div>
                </div>
                <div class="map-container" id="mapContainer">
                    <div class="map-overlay">
                        <span id="mapModeIndicator">Online</span>
                        <span id="mapLayerLabel">Street</span>
                    </div>
                    <div id="onlineMap"></div>
                    <div id="offlineGrid"></div>
                </div>
            </section>

            <section id="network-system" class="control-section panel-control">
                <div class="card">
                    <h3><i class="fas fa-wifi"></i> Network &amp; System</h3>
                    <div class="info-grid">
                        <div class="info-item">
                            <span class="info-label">Mode</span>
                            <span class="info-value" id="wifiMode">AP</span>
                        </div>
                        <div class="info-item">
                            <span class="info-label">SSID</span>
                            <span class="info-value" id="apSSID">AI-TRAC</span>
                        </div>
                        <div class="info-item">
                            <span class="info-label">IP</span>
                            <span class="info-value" id="apIP">192.168.4.1</span>
                        </div>
                        <div class="info-item">
                            <span class="info-label">Sinyal</span>
                            <div class="info-value">
                                <div class="signal-indicator">
                                    <div class="signal-bar active"></div>
                                    <div class="signal-bar active"></div>
                                    <div class="signal-bar active medium"></div>
                                    <div class="signal-bar"></div>
                                </div>
                                <span id="signalStrength">Baik</span>
                            </div>
                        </div>
                        <div class="info-item">
                            <span class="info-label">Terhubung</span>
                            <span class="info-value" id="connectedSSID">-</span>
                        </div>
                        <div class="info-item">
                            <span class="info-label">Status</span>
                            <span class="info-value" id="wifiStatus">AP Aktif</span>
                        </div>
                    </div>
                    <div class="wifi-scan-container">
                        <div class="button-group">
                            <button class="btn btn-primary" id="scanWifi"><i class="fas fa-wifi"></i> Scan</button>
                            <button class="btn btn-secondary" id="refreshWifi"><i class="fas fa-redo"></i> Ulang</button>
                        </div>
                        <div id="wifiList" class="wifi-list" style="display: none;">
                            <p style="text-align: center; color: var(--text-muted); font-size: 0.8rem; padding: 12px;">Klik Scan untuk deteksi jaringan</p>
                        </div>
                    </div>
                </div>

                <div class="card">
                    <h3><i class="fas fa-clock"></i> Waktu &amp; GPS</h3>
                    <div class="gps-status">
                        <div class="gps-status-indicator" id="gpsStatus"></div>
                        <span id="gpsStatusText">GPS Searching...</span>
                    </div>
                    <div class="time-display" id="currentTime">--:--:--</div>
                    <div class="time-source" id="timeSource">Source: GPS</div>
                    
                    <div class="time-selector">
                        <div class="info-grid">
                            <div class="info-item">
                                <span class="info-label">Latitude</span>
                                <span class="info-value" id="gpsLat">0.0000°</span>
                            </div>
                            <div class="info-item">
                                <span class="info-label">Longitude</span>
                                <span class="info-value" id="gpsLon">0.0000°</span>
                            </div>
                        </div>
                        <button class="btn btn-secondary" id="syncTime">
                            <i class="fas fa-sync-alt"></i> Sinkron GPS
                        </button>
                    </div>
                </div>

                <div id="control-mode" class="card">
                    <h3><i class="fas fa-gamepad"></i> Control Mode</h3>
                    <div class="mode-selector">
                        <button class="mode-btn active" id="modeManual"><i class="fas fa-joystick"></i> Manual</button>
                        <button class="mode-btn" id="modeLine"><i class="fas fa-stream"></i> Line</button>
                        <button class="mode-btn" id="modeAuto"><i class="fas fa-route"></i> Auto</button>
                    </div>
                    <p class="section-hint">Manual: kendali remote • Line: semi-otonom • Auto: ikuti waypoint. Pilih mode lalu gunakan joystick (Manual) atau tombol navigasi.</p>
                    <p class="control-status-text" id="controlStatusText" style="font-size:0.75rem;margin-top:6px;color:var(--text-muted);">Manual Mode Active</p>

                    <div class="joystick-zone" id="joystickZone" style="display:none;">
                        <p style="font-size:0.7rem;color:var(--text-muted);margin-bottom:6px;">Joystick Virtual (Manual)</p>
                        <div class="joystick-container" id="joystickContainer">
                            <div class="joystick-stick" id="joystickStick"></div>
                        </div>
                    </div>
                    <div class="control-values">
                        <div class="control-value">
                            <div class="control-label">Throttle</div>
                            <div class="control-number" id="throttleValue">0%</div>
                        </div>
                        <div class="control-value">
                            <div class="control-label">Steering</div>
                            <div class="control-number" id="steeringValue">0%</div>
                        </div>
                    </div>

                    <div class="control-stats">
                        <div class="stat-item">
                            <div class="stat-label">Speed</div>
                            <div class="stat-value"><span id="speedValue">0.0</span><span class="stat-unit">km/h</span></div>
                        </div>
                        <div class="stat-item">
                            <div class="stat-label">Heading</div>
                            <div class="stat-value"><span id="headingValue">0</span><span class="stat-unit">°</span></div>
                        </div>
                        <div class="stat-item">
                            <div class="stat-label">GPS Sats</div>
                            <div class="stat-value"><span id="gpsSatsValue">0</span></div>
                        </div>
                        <div class="stat-item">
                            <div class="stat-label">Sensors (cm)</div>
                            <div class="stat-value"><span id="ultrasonicValue">-/-/-</span></div>
                        </div>
                    </div>

                    <div class="navigation-controls">
                        <button class="btn btn-secondary" id="forwardControl"><i class="fas fa-arrow-up"></i> Maju</button>
                        <button class="btn btn-secondary" id="backwardControl"><i class="fas fa-arrow-down"></i> Mundur</button>
                        <button class="btn btn-emergency" id="emergencyStop"><i class="fas fa-exclamation-triangle"></i> EMERGENCY STOP</button>
                    </div>
                </div>
            </section>

            <section id="waypoint-manager" class="card panel-waypoint">
                <h3><i class="fas fa-map-pin"></i> Waypoint Manager</h3>
                <p class="section-hint" style="margin-bottom:var(--space-sm);">Klik di peta untuk menambah waypoint. Urutan: WP1 → WP2 → WP3… Polygon area lahan harus tertutup (min. 3 titik).</p>
                <ul class="waypoint-list" id="waypointList">
                    <li style="opacity:0.8;color:var(--text-muted);">Belum ada waypoint — klik di peta untuk menambah</li>
                </ul>
                <div class="waypoint-validation valid" id="waypointValidation">Klik di peta untuk menambah waypoint. Minimal 3 titik agar jalur tertutup (polygon).</div>
                <div class="button-group" style="margin-top:10px;">
                    <button class="btn btn-secondary" id="clearWaypointsList"><i class="fas fa-trash-alt"></i> Hapus waypoint</button>
                </div>
            </section>

            <!-- Monitoring Lahan - Kamera + ML -->
            <section id="monitoring-lahan" class="monitoring-lahan">
                <p class="section-hint" style="margin-bottom:var(--space-sm);grid-column:1/-1;">Feed kamera dan deteksi objek (tanaman, gulma, air, hambatan) untuk monitoring lahan.</p>
                <div class="camera-feed-container">
                    <h3 style="padding:var(--space-md) var(--space-lg);border-bottom:1px solid var(--border);font-size:1rem;"><i class="fas fa-video"></i> Kamera Real-Time</h3>
                    <div class="camera-feed" id="cameraFeed">
                        <span><i class="fas fa-video-slash"></i> Feed kamera (sambungkan sumber video)</span>
                    </div>
                    <div class="ml-overlay" id="mlOverlay"></div>
                </div>
                <div class="insight-monitoring">
                    <div class="insight-card">
                        <h4><i class="fas fa-robot"></i> Deteksi ML</h4>
                        <div class="obj-list" id="mlObjectList">Tanaman: 0 | Gulma: 0 | Air: 0 | Hambatan: 0</div>
                        <div class="status-lahan normal" id="statusLahan">Status Lahan: Normal</div>
                    </div>
                    <div class="insight-card">
                        <h4>Objek Terdeteksi</h4>
                        <p class="value" id="mlObjectCount" style="font-size:1.5rem;font-weight:700;color:var(--primary);">0</p>
                        <p style="font-size:0.75rem;color:var(--text-muted);">Bounding box + label di video</p>
                    </div>
                </div>
            </section>

            <section id="history-analytics" class="usage-log panel-history">
                <h3><i class="fas fa-chart-line"></i> History &amp; Analytics</h3>
                <p class="section-hint" style="margin-bottom:var(--space-sm);">Lihat riwayat sesi, grafik, dan replay. Filter menurut mode atau tanggal.</p>
                <div class="history-filters">
                    <select id="filterMode" title="Filter by mode"><option value="">Semua Mode</option><option value="manual">Manual</option><option value="line">Line</option><option value="auto">Auto</option></select>
                    <select id="filterDate" title="Filter by date"><option value="">Semua Tanggal</option></select>
                </div>
                <div class="history-tabs">
                    <button class="history-tab active" data-view="list">List</button>
                    <button class="history-tab" data-view="graph">Grafik</button>
                    <button class="history-tab" data-view="replay">Replay Lintasan</button>
                </div>
                <div id="historyList" class="history-view active">
                    <div class="log-controls">
                        <div class="button-group">
                            <button class="btn btn-secondary" id="clearLog"><i class="fas fa-trash"></i> Hapus</button>
                            <button class="btn btn-secondary" id="exportLog"><i class="fas fa-download"></i> Export</button>
                        </div>
                        <div class="battery-indicator">
                            <div class="battery-level">
                                <div class="battery-fill battery-high" id="batteryFill" style="width: 85%"></div>
                            </div>
                            <span class="battery-percentage" id="batteryPercentage">85%</span>
                        </div>
                    </div>
                    <div class="log-table-container">
                        <table class="log-table">
                            <thead>
                                <tr>
                                    <th>Waktu</th>
                                    <th>Mode</th>
                                    <th>Jarak (m)</th>
                                    <th>Kecepatan</th>
                                    <th>Baterai</th>
                                </tr>
                            </thead>
                            <tbody id="logTableBody"></tbody>
                        </table>
                    </div>
                </div>
                <div id="historyGraph" class="history-view">
                    <div class="chart-container">
                        <canvas id="logChart" width="400" height="180"></canvas>
                    </div>
                </div>
                <div id="historyReplay" class="history-view">
                    <p style="font-size:0.85rem;opacity:0.9;">Replay lintasan dari riwayat perjalanan di peta.</p>
                    <div class="replay-controls">
                        <button class="btn btn-secondary" id="replayStart" style="padding:6px 10px;">
                            <i class="fas fa-play"></i> Putar
                        </button>
                        <input type="range" class="replay-slider" id="replaySlider" min="0" max="100" value="0">
                        <span id="replayTime">0:00</span>
                    </div>
                    <p id="replayHint" style="font-size:0.75rem;opacity:0.7;margin-top:8px;">Jalankan operasi dulu agar ada data replay.</p>
                </div>
            </section>
                </div>
            </main>

                <!-- Settings Page - System, Network, Control, AI/ML, UI -->
                <section id="settings" class="settings-panel" style="padding: 20px;">
                    <h2 style="margin-bottom:16px;font-size:1.25rem;"><i class="fas fa-cog"></i> Settings</h2>
                    <div class="settings-tabs">
                        <button class="settings-tab active" data-settings="system">System</button>
                        <button class="settings-tab" data-settings="network">Network</button>
                        <button class="settings-tab" data-settings="control">Control</button>
                        <button class="settings-tab" data-settings="aiml">AI / ML</button>
                        <button class="settings-tab" data-settings="ui">UI</button>
                    </div>
                    <div class="settings-section active" id="settingsSystem">
                        <h3>System Settings</h3>
                        <div class="settings-row"><label>Kalibrasi GPS</label><button class="btn btn-secondary btn-sm">Kalibrasi</button></div>
                        <div class="settings-row"><label>Update Firmware</label><button class="btn btn-secondary btn-sm">Cek Update</button></div>
                        <div class="settings-row"><label>Reset Sistem</label><button class="btn btn-danger btn-sm">Reset</button></div>
                    </div>
                    <div class="settings-section" id="settingsNetwork">
                        <h3>Network Settings</h3>
                        <div class="settings-row"><label>Scan WiFi</label><button class="btn btn-primary btn-sm" id="settingsScanWifi">Scan</button></div>
                        <div class="settings-row"><label>Mode</label><select><option>STA</option><option>AP</option></select></div>
                        <div class="settings-row"><label>Auto Reconnect</label><input type="checkbox" checked></div>
                    </div>
                    <div class="settings-section" id="settingsControl">
                        <h3>Control Settings</h3>
                        <div class="settings-row"><label>Sensitivitas Joystick</label><input type="range" min="1" max="10" value="5"></div>
                        <div class="settings-row"><label>Kecepatan Traktor</label><input type="number" value="50" min="1" max="100"> %</div>
                        <div class="settings-row"><label>Threshold Autonomous</label><input type="number" value="80" min="0" max="100"> %</div>
                    </div>
                    <div class="settings-section" id="settingsAiml">
                        <h3>AI / ML Settings</h3>
                        <div class="settings-row"><label>Enable Detection</label><input type="checkbox" checked></div>
                        <div class="settings-row"><label>Confidence Threshold</label><input type="number" value="70" min="0" max="100"> %</div>
                        <div class="settings-row"><label>Jenis Objek Aktif</label><select multiple><option>Tanaman</option><option>Gulma</option><option>Air</option><option>Hambatan</option></select></div>
                    </div>
                    <div class="settings-section" id="settingsUi">
                        <h3>UI Settings</h3>
                        <div class="settings-row"><label>Dark / Light Mode</label><div class="theme-toggle" id="themeToggleSettings"><i class="fas fa-sun"></i> Terang</div></div>
                        <div class="settings-row"><label>Bahasa</label><select><option>Indonesia</option><option>English</option></select></div>
                        <div class="settings-row"><label>Ukuran Font</label><select><option>Kecil</option><option selected>Normal</option><option>Besar</option></select></div>
                    </div>
                </section>

                <footer class="footer-info" style="padding: 12px 20px; margin: 0; border-top: 1px solid var(--border);">
                <span>Web Dashboard Traktor Otonom • IoT, GPS, Kamera &amp; ML</span>
                <span>RnD IPB • Smart Agriculture</span>
            </footer>
        </div>
    </div>

    <!-- Password Modal -->
    <div class="password-modal" id="passwordModal">
        <div class="password-content">
            <h3 id="modalSSID">Connect to WiFi</h3>
            <input type="password" class="password-input" id="wifiPassword" placeholder="Enter WiFi password" autocomplete="off">
            <div class="password-buttons">
                <button class="btn btn-secondary" id="cancelConnect">Cancel</button>
                <button class="btn btn-primary" id="confirmConnect">Connect</button>
            </div>
        </div>
    </div>

    <div class="toast" id="toast">
        <i class="fas fa-check-circle"></i>
        <span id="toastMessage">Action completed</span>
    </div>

    <script src="https://unpkg.com/leaflet@1.9.4/dist/leaflet.js"></script>
    <script>
        // DOM Elements
        const themeToggle = document.getElementById('themeToggle');
        const body = document.body;
        const currentTimeElement = document.getElementById('currentTime');
        const timeSourceElement = document.getElementById('timeSource');
        const throttleValue = document.getElementById('throttleValue');
        const steeringValue = document.getElementById('steeringValue');
        const speedValue = document.getElementById('speedValue');
        const headingValue = document.getElementById('headingValue');
        const gpsSatsValue = document.getElementById('gpsSatsValue');
        const ultrasonicValue = document.getElementById('ultrasonicValue');
        const gpsLat = document.getElementById('gpsLat');
        const gpsLon = document.getElementById('gpsLon');
        const gpsStatus = document.getElementById('gpsStatus');
        const gpsStatusText = document.getElementById('gpsStatusText');
        const toast = document.getElementById('toast');
        const toastMessage = document.getElementById('toastMessage');
        const batteryFill = document.getElementById('batteryFill');
        const batteryPercentage = document.getElementById('batteryPercentage');
        const logTableBody = document.getElementById('logTableBody');
        const connectedSSID = document.getElementById('connectedSSID');
        
        // WiFi elements
        const wifiMode = document.getElementById('wifiMode');
        const apSSID = document.getElementById('apSSID');
        const apIP = document.getElementById('apIP');
        const wifiStatus = document.getElementById('wifiStatus');
        const signalStrength = document.getElementById('signalStrength');
        const scanWifiBtn = document.getElementById('scanWifi');
        const refreshWifiBtn = document.getElementById('refreshWifi');
        const wifiList = document.getElementById('wifiList');
        
        // Password modal elements
        const passwordModal = document.getElementById('passwordModal');
        const modalSSID = document.getElementById('modalSSID');
        const wifiPassword = document.getElementById('wifiPassword');
        const cancelConnectBtn = document.getElementById('cancelConnect');
        const confirmConnectBtn = document.getElementById('confirmConnect');
        
        // Mode buttons
        const modeManual = document.getElementById('modeManual');
        const modeAuto = document.getElementById('modeAuto');
        const modeLine = document.getElementById('modeLine');
        
        // Control buttons
        const centerMapBtn = document.getElementById('centerMap');
        const clearWaypointsBtn = document.getElementById('clearWaypoints');
        const startNavigationBtn = document.getElementById('startNavigation');
        const toggleMapModeBtn = document.getElementById('toggleMapMode');
        const syncTimeBtn = document.getElementById('syncTime');
        const forwardControlBtn = document.getElementById('forwardControl');
        const backwardControlBtn = document.getElementById('backwardControl');
        const emergencyStopBtn = document.getElementById('emergencyStop');
        const clearLogBtn = document.getElementById('clearLog');
        const exportLogBtn = document.getElementById('exportLog');
        
        // Map elements
        const mapContainer = document.getElementById('mapContainer');
        const onlineMap = document.getElementById('onlineMap');
        const offlineGrid = document.getElementById('offlineGrid');
        const mapTitle = document.getElementById('mapTitle');
        const mapModeIndicator = document.getElementById('mapModeIndicator');
        
        // Variables
        let currentMode = 'manual';
        let throttle = 0;
        let steering = 0;
        let navigationActive = false;
        let waypoints = [];
        let isScanning = false;
        let isOnline = true;
        let map = null;
        let vehicleMarker = null;
        let vehiclePosition = {lat: -6.5589, lng: 106.8065}; // Default position (IPB Bogor)
        let vehicleHeading = 0;
        let gridScale = 10;
        let totalDistance = 0;
        let logEntries = [];
        let batteryLevel = 85;
        let trackPoints = [];
        let pathPolyline = null;
        let waypointPolyline = null;
        let tileLayerStreet = null;
        let tileLayerSatellite = null;
        let currentTileLayer = 'street';
        let currentWifiNetworks = [];
        let selectedSSID = '';
        let isESP32Connected = false;
        
        // Initialize theme (default: dark/futuristik)
        function initTheme() {
            const savedTheme = localStorage.getItem('ai-trac-theme') || 'dark';
            if (savedTheme === 'light') {
                body.classList.add('light-theme');
                themeToggle.innerHTML = '<i class="fas fa-moon"></i><span>Gelap</span>';
            } else {
                body.classList.remove('light-theme');
                themeToggle.innerHTML = '<i class="fas fa-sun"></i><span>Terang</span>';
            }
        }
        
        themeToggle.addEventListener('click', function() {
            if (body.classList.contains('light-theme')) {
                body.classList.remove('light-theme');
                themeToggle.innerHTML = '<i class="fas fa-sun"></i><span>Terang</span>';
                localStorage.setItem('ai-trac-theme', 'dark');
                showToast('Tema gelap');
            } else {
                body.classList.add('light-theme');
                themeToggle.innerHTML = '<i class="fas fa-moon"></i><span>Gelap</span>';
                localStorage.setItem('ai-trac-theme', 'light');
                showToast('Tema terang');
            }
        });

        // Real-time status bar from /getData
        let lastWifiConnected = false;
        function updateStatusBar(data) {
            if (!data) return;
            const gpsEl = document.getElementById('statusGps');
            const gpsVal = document.getElementById('statusGpsVal');
            const wifiEl = document.getElementById('statusWifi');
            const wifiVal = document.getElementById('statusWifiVal');
            const modeVal = document.getElementById('statusModeVal');
            const batEl = document.getElementById('statusBattery');
            const timeEl = document.getElementById('statusTime');
            if (gpsEl) {
                const fix = data.gpsSats > 0;
                gpsEl.classList.toggle('online', fix);
                gpsVal.textContent = fix ? (data.gpsSats + ' sat') : 'No fix';
            }
            if (wifiEl && wifiVal) {
                const connected = data.wifiConnected || data.isAPMode;
                wifiEl.classList.remove('weak', 'off');
                if (data.isAPMode) { wifiEl.classList.add('online'); wifiVal.textContent = 'AP'; }
                else if (connected) { wifiEl.classList.add('online'); wifiVal.textContent = (data.wifiSSID || 'STA').substring(0,12); }
                else { wifiEl.classList.add('off'); wifiVal.textContent = 'Off'; }
                if (lastWifiConnected && !connected && !data.isAPMode) onWiFiDisconnected();
                lastWifiConnected = !!(data.wifiConnected || data.isAPMode);
            }
            if (modeVal) modeVal.textContent = data.mode || 'MANUAL';
            if (batEl) batEl.textContent = (batteryLevel != null ? Math.round(batteryLevel) : '--') + '%';
            if (timeEl) timeEl.textContent = data.currentTime || '--:--:--';
        }
        function onWiFiDisconnected() {
            showToast('WiFi terputus. Rescan & reconnect...', 'warning');
            setTimeout(function() { detectRealWiFiNetworks(); }, 500);
        }
        
        // Time Management with GPS
        function updateTime() {
            // Try to get GPS time from ESP32 first
            fetch('/api/time/gps')
                .then(response => response.json())
                .then(data => {
                    if (data.time && data.time !== '--:--:--') {
                        currentTimeElement.textContent = data.time;
                        timeSourceElement.textContent = 'Source: GPS';
                        
                        if (data.date) {
                            // Store full timestamp for logging
                            const timestamp = `${data.date} ${data.time}`;
                            localStorage.setItem('lastGPSTime', timestamp);
                        }
                    } else {
                        // Fallback to system time
                        const now = new Date();
                        const timeStr = now.toTimeString().substring(0, 8);
                        currentTimeElement.textContent = timeStr;
                        timeSourceElement.textContent = 'Source: System';
                    }
                    
                    // Update GPS status
                    if (data.fix) {
                        gpsStatus.classList.add('active');
                        gpsStatusText.textContent = `GPS Fix (${data.sats} sats)`;
                        gpsSatsValue.textContent = data.sats;
                        
                            if (data.lat && data.lon) {
                            gpsLat.textContent = `${parseFloat(data.lat).toFixed(4)}°`;
                            gpsLon.textContent = `${parseFloat(data.lon).toFixed(4)}°`;
                            
                            const lat = parseFloat(data.lat);
                            const lng = parseFloat(data.lon);
                            vehiclePosition.lat = lat;
                            vehiclePosition.lng = lng;
                            addTrackPoint(lat, lng);
                            
                            if (map && isOnline) {
                                vehicleMarker.setLatLng(vehiclePosition);
                                if (data.heading) {
                                    vehicleHeading = parseFloat(data.heading);
                                    vehicleMarker.style.setProperty('--heading', `${vehicleHeading}deg`);
                                }
                            } else if (!isOnline) {
                                updateOfflineGridPosition();
                            }
                        }
                    } else {
                        gpsStatus.classList.remove('active');
                        gpsStatusText.textContent = 'GPS Searching...';
                    }
                })
                .catch(error => {
                    console.error('Error updating GPS time:', error);
                    const now = new Date();
                    currentTimeElement.textContent = now.toTimeString().substring(0, 8);
                    timeSourceElement.textContent = 'Source: System';
                    gpsStatus.classList.remove('active');
                    gpsStatusText.textContent = 'GPS Offline';
                });
        }
        
        // Initialize time updates every second
        setInterval(updateTime, 1000);
        updateTime(); // Initial call

        // Poll device data for status bar and overview
        function pollGetData() {
            fetch('/getData').then(r => r.json()).then(function(data) {
                updateStatusBar(data);
                if (data.currentTime) { var el = document.getElementById('statusTime'); if (el) el.textContent = data.currentTime; }
                if (data.mode) { var el = document.getElementById('statusModeVal'); if (el) el.textContent = data.mode; }
                var op = document.getElementById('overviewPosition');
                if (op) op.textContent = (vehiclePosition.lat ? vehiclePosition.lat.toFixed(4) : '--') + ', ' + (vehiclePosition.lng ? vehiclePosition.lng.toFixed(4) : '--');
                var om = document.getElementById('overviewMode');
                if (om) om.textContent = data.mode || currentMode.toUpperCase();
                var oc = document.getElementById('overviewConnection');
                if (oc) oc.textContent = (data.gpsSats > 0 ? 'GPS OK' : 'GPS --') + ' / ' + (data.wifiConnected || data.isAPMode ? 'WiFi OK' : 'WiFi --');
                var healthEl = document.getElementById('statusHealthVal');
                var healthItem = document.getElementById('statusHealthItem');
                if (healthEl && healthItem) {
                    var ok = (data.gpsSats > 0) && (data.wifiConnected || data.isAPMode);
                    healthEl.textContent = ok ? 'OK' : 'Warning';
                    healthItem.classList.remove('health-ok', 'health-warn', 'health-error');
                    healthItem.classList.add(ok ? 'health-ok' : 'health-warn');
                }
            }).catch(function() {});
        }
        setInterval(pollGetData, 2000);
        pollGetData();
        document.getElementById('overviewStart') && document.getElementById('overviewStart').addEventListener('click', function() { document.getElementById('startNavigation') && document.getElementById('startNavigation').click(); });
        document.getElementById('overviewStop') && document.getElementById('overviewStop').addEventListener('click', function() { document.getElementById('stopNavigation') && document.getElementById('stopNavigation').click(); });
        document.getElementById('overviewEmergency') && document.getElementById('overviewEmergency').addEventListener('click', function() { document.getElementById('emergencyStop') && document.getElementById('emergencyStop').click(); });
        
        // Mode Control
        function setMode(mode) {
            // Remove active class from all buttons
            modeManual.classList.remove('active');
            modeAuto.classList.remove('active');
            modeLine.classList.remove('active');
            
            // Add active class to selected mode
            var joystickZone = document.getElementById('joystickZone');
            var controlStatusText = document.getElementById('controlStatusText');
            if (mode === 'manual') {
                modeManual.classList.add('active');
                currentMode = 'manual';
                if (joystickZone) joystickZone.style.display = 'block';
                if (controlStatusText) controlStatusText.textContent = 'Manual Mode Active';
                showToast('Mode changed to MANUAL');
                logModeChange('manual');
                sendModeCommand('manual');
            } else if (mode === 'auto') {
                modeAuto.classList.add('active');
                currentMode = 'auto';
                if (joystickZone) joystickZone.style.display = 'none';
                if (controlStatusText) controlStatusText.textContent = 'Auto Mode Active';
                showToast('Mode changed to AUTO');
                logModeChange('auto');
                sendModeCommand('auto');
            } else if (mode === 'line') {
                modeLine.classList.add('active');
                currentMode = 'line';
                if (joystickZone) joystickZone.style.display = 'none';
                if (controlStatusText) controlStatusText.textContent = 'Line Mode Active';
                showToast('Mode changed to LINE');
                logModeChange('line');
                sendModeCommand('line');
            }
        }
        
        modeManual.addEventListener('click', function() { setMode('manual'); });
        modeAuto.addEventListener('click', function() { setMode('auto'); });
        modeLine.addEventListener('click', function() { setMode('line'); });
        
        // WiFi: Scan real only via ESP32. Jika putus: reset, scan ulang, reconnect.
        scanWifiBtn.addEventListener('click', function() {
            if (isScanning) return;
            isScanning = true;
            wifiList.style.display = 'block';
            wifiList.innerHTML = '<p style="text-align: center; opacity: 0.7; font-size: 0.8rem;"><i class="fas fa-spinner fa-spin"></i> Scan WiFi real dari perangkat...</p>';
            scanWithESP32();
        });
        
        refreshWifiBtn.addEventListener('click', function() {
            if (isScanning) return;
            wifiList.style.display = 'block';
            wifiList.innerHTML = '<p style="text-align: center; opacity: 0.7; font-size: 0.8rem;"><i class="fas fa-spinner fa-spin"></i> Scan ulang...</p>';
            isScanning = true;
            scanWithESP32();
        });
        
        function detectRealWiFiNetworks() {
            isScanning = true;
            wifiList.style.display = 'block';
            wifiList.innerHTML = '<p style="text-align: center; opacity: 0.7; font-size: 0.8rem;"><i class="fas fa-spinner fa-spin"></i> Scan jaringan WiFi aktif...</p>';
            scanWithESP32();
        }
        
        function scanWithESP32() {
            fetch('/api/wifi/scan')
                .then(response => {
                    if (!response.ok) throw new Error('Perangkat tidak merespon');
                    return response.json();
                })
                .then(data => {
                    isScanning = false;
                    const list = data.networks || [];
                    if (list.length > 0) {
                        const nets = list.map(n => ({
                            ssid: n.ssid || '',
                            rssi: n.rssi || -99,
                            security: (n.security === 'Open') ? 'Open' : 'WPA2',
                            channel: n.channel || 1,
                            bssid: n.bssid || ''
                        }));
                        displayWiFiNetworks(nets, 'Scan perangkat');
                    } else {
                        wifiList.innerHTML = '<p style="text-align: center; opacity: 0.7; font-size: 0.8rem;">Tidak ada jaringan WiFi terdeteksi.</p>';
                        showToast('Tidak ada jaringan terdeteksi', 'warning');
                    }
                })
                .catch(err => {
                    isScanning = false;
                    wifiList.innerHTML = '<p style="text-align: center; opacity: 0.7; font-size: 0.8rem;">Gagal scan. Periksa koneksi ke perangkat.</p>';
                    showToast('Gagal scan WiFi', 'error');
                });
        }
        
        function displayWiFiNetworks(networks, source) {
            currentWifiNetworks = networks;
            
            if (networks.length > 0) {
                wifiList.innerHTML = '';
                
                // Sort by signal strength (strongest first)
                networks.sort((a, b) => b.rssi - a.rssi);
                
                networks.forEach((network, index) => {
                    const wifiItem = document.createElement('div');
                    wifiItem.className = 'wifi-item';
                    
                    // Check if this is the connected network
                    const isConnected = network.ssid === localStorage.getItem('connectedWiFiSSID');
                    if (isConnected) {
                        wifiItem.classList.add('connected');
                    }
                    
                    // Determine signal strength category
                    let signalCategory = 'weak';
                    let signalStrengthText = 'Weak';
                    if (network.rssi > -50) {
                        signalCategory = 'excellent';
                        signalStrengthText = 'Excellent';
                    } else if (network.rssi > -60) {
                        signalCategory = 'good';
                        signalStrengthText = 'Good';
                    } else if (network.rssi > -70) {
                        signalCategory = 'fair';
                        signalStrengthText = 'Fair';
                    }
                    
                    // Determine encryption type display
                    const encryptionType = network.security === 'Open' ? 'Open' : 'Secured';
                    const encryptionClass = network.security === 'Open' ? 'wifi-open' : '';
                    
                    wifiItem.innerHTML = `
                        <div class="wifi-info">
                            <div class="wifi-ssid">
                                ${network.hidden ? '<i class="fas fa-eye-slash"></i>' : '<i class="fas fa-wifi"></i>'}
                                ${network.ssid || 'Hidden Network'}
                                ${isConnected ? '<span style="color: var(--connected-color);"><i class="fas fa-link"></i></span>' : ''}
                                <span class="wifi-encryption ${encryptionClass}">${encryptionType}</span>
                                <span class="wifi-channel">Ch ${network.channel}</span>
                            </div>
                            <div class="wifi-signal">
                                <span class="signal-strength ${signalCategory}">${signalStrengthText}</span>
                                <span>${network.rssi} dBm</span>
                                <span style="opacity: 0.5;">|</span>
                                <span>${network.bssid || 'Unknown BSSID'}</span>
                            </div>
                        </div>
                        <div class="wifi-actions">
                            ${!isConnected ? `
                                <button class="btn btn-primary wifi-connect-btn" data-ssid="${network.ssid || ''}" data-security="${network.security || 'Open'}" data-bssid="${network.bssid || ''}">
                                    Connect
                                </button>
                            ` : `
                                <button class="btn btn-danger wifi-connect-btn" data-ssid="${network.ssid || ''}">
                                    Disconnect
                                </button>
                            `}
                        </div>
                    `;
                    
                    wifiList.appendChild(wifiItem);
                });
                
                // Add event listeners for connect buttons
                document.querySelectorAll('.wifi-connect-btn').forEach(btn => {
                    btn.addEventListener('click', function() {
                        const ssid = this.getAttribute('data-ssid');
                        const security = this.getAttribute('data-security');
                        const bssid = this.getAttribute('data-bssid');
                        
                        if (this.textContent === 'Disconnect') {
                            disconnectFromWiFi(ssid);
                        } else {
                            selectedSSID = ssid;
                            showPasswordModal(ssid, security, bssid);
                        }
                    });
                });
                
                showToast(`Found ${networks.length} real networks (${source})`);
            } else {
                wifiList.innerHTML = '<p style="text-align: center; opacity: 0.7; font-size: 0.8rem;">No WiFi networks detected. Make sure WiFi is enabled.</p>';
                showToast('No networks found', 'warning');
            }
        }
        
        function showPasswordModal(ssid, security, bssid) {
            modalSSID.textContent = `Connect to "${ssid}"`;
            wifiPassword.value = '';
            
            // Auto-fill saved password if exists
            const savedPassword = localStorage.getItem(`wifi_password_${ssid}`);
            if (savedPassword) {
                wifiPassword.value = savedPassword;
            }
            
            passwordModal.classList.add('show');
            wifiPassword.focus();
            
            // Store network info in modal dataset
            passwordModal.dataset.ssid = ssid;
            passwordModal.dataset.security = security;
            passwordModal.dataset.bssid = bssid;
        }
        
        function hidePasswordModal() {
            passwordModal.classList.remove('show');
            selectedSSID = '';
        }
        
        function connectToWiFi(ssid, password, security, bssid) {
            if (!ssid) {
                showToast('Cannot connect to hidden network', 'warning');
                return;
            }
            
            showToast(`Connecting to ${ssid}...`);
            
            // Save password for future use
            if (password) {
                localStorage.setItem(`wifi_password_${ssid}`, password);
            }
            
            // Try to connect via ESP32 API
            fetch('/api/wifi/connect', {
                method: 'POST',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify({ 
                    ssid: ssid, 
                    password: password,
                    bssid: bssid,
                    security: security
                })
            })
            .then(response => response.json())
            .then(data => {
                if (data.ok || data.status === 'connecting' || data.status === 'connected') {
                    connectedSSID.textContent = ssid;
                    wifiStatus.textContent = 'Menghubungkan...';
                    localStorage.setItem('connectedWiFiSSID', ssid);
                    hidePasswordModal();
                    checkWiFiConnectionStatus(ssid);
                }
            })
            .catch(error => {
                console.error('Connection error:', error);
                
                // Simulate connection for demo purposes
                setTimeout(() => {
                    connectedSSID.textContent = ssid;
                    wifiStatus.textContent = 'Connected (STA)';
                    localStorage.setItem('connectedWiFiSSID', ssid);
                    hidePasswordModal();
                    showToast(`Connected to ${ssid}!`, 'success');
                    
                    // Refresh network list
                    detectRealWiFiNetworks();
                    
                    // Check internet connectivity
                    checkInternetConnectivity();
                }, 2000);
            });
        }
        
        function disconnectFromWiFi(ssid) {
            fetch('/api/wifi/disconnect', {
                method: 'POST',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify({ ssid: ssid })
            })
            .then(response => response.json())
            .then(data => {
                connectedSSID.textContent = 'Not connected';
                wifiStatus.textContent = 'AP Active';
                localStorage.removeItem('connectedWiFiSSID');
                showToast(`Disconnected from ${ssid}`, 'warning');
                
                // Refresh network list
                detectRealWiFiNetworks();
            })
            .catch(error => {
                console.error('Disconnection error:', error);
                
                // Simulate for demo
                connectedSSID.textContent = 'Not connected';
                wifiStatus.textContent = 'AP Active';
                localStorage.removeItem('connectedWiFiSSID');
                showToast(`Disconnected from ${ssid}`, 'warning');
                detectRealWiFiNetworks();
            });
        }
        
        function checkWiFiConnectionStatus(ssid) {
            let attempts = 0;
            const maxAttempts = 25;
            const checkInterval = setInterval(() => {
                fetch('/getData')
                    .then(r => r.json())
                    .then(data => {
                        attempts++;
                        if (data.wifiConnected && (data.wifiSSID || '').indexOf(ssid) !== -1) {
                            clearInterval(checkInterval);
                            showToast('Terhubung ke ' + ssid + (data.wifiIP ? ' - ' + data.wifiIP : ''), 'success');
                            if (document.getElementById('apIP')) document.getElementById('apIP').textContent = data.wifiIP || 'STA';
                            checkInternetConnectivity();
                        } else if (attempts >= maxAttempts) {
                            clearInterval(checkInterval);
                            showToast('Koneksi timeout', 'error');
                        }
                    })
                    .catch(() => {
                        clearInterval(checkInterval);
                        showToast('Cek koneksi gagal', 'error');
                    });
            }, 1000);
        }
        
        // Password modal handlers
        cancelConnectBtn.addEventListener('click', hidePasswordModal);
        
        confirmConnectBtn.addEventListener('click', function() {
            const ssid = passwordModal.dataset.ssid;
            const security = passwordModal.dataset.security;
            const bssid = passwordModal.dataset.bssid;
            const password = wifiPassword.value;
            
            if (!password && security !== 'Open') {
                showToast('Password required for secured network', 'error');
                wifiPassword.focus();
                return;
            }
            
            connectToWiFi(ssid, password, security, bssid);
        });
        
        wifiPassword.addEventListener('keypress', function(e) {
            if (e.key === 'Enter') {
                confirmConnectBtn.click();
            }
        });
        
        // Close modal when clicking outside
        passwordModal.addEventListener('click', function(e) {
            if (e.target === passwordModal) {
                hidePasswordModal();
            }
        });
        
        // Internet Connectivity Check
        function checkInternetConnectivity() {
            // Try to load Leaflet tiles as connectivity test
            const testImage = new Image();
            testImage.onload = function() {
                isOnline = true;
                showToast('Internet connection detected', 'success');
                switchToOnlineMap();
            };
            testImage.onerror = function() {
                isOnline = false;
                showToast('No internet connection, using offline mode', 'warning');
                switchToOfflineGrid();
            };
            testImage.src = 'https://unpkg.com/leaflet@1.9.4/dist/images/marker-icon.png';
        }
        
        // Map: Online = street/satellite, lintasan real-time (polyline). Offline = grid X-Y.
        function initOnlineMap() {
            if (!map) {
                map = L.map('onlineMap').setView([vehiclePosition.lat, vehiclePosition.lng], 18);
                tileLayerStreet = L.tileLayer('https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png', {
                    attribution: '© OSM', maxZoom: 19
                });
                tileLayerSatellite = L.tileLayer('https://server.arcgisonline.com/ArcGIS/rest/services/World_Imagery/MapServer/tile/{z}/{y}/{x}', {
                    attribution: '© Esri', maxZoom: 19
                });
                tileLayerStreet.addTo(map);
                currentTileLayer = 'street';
                if (document.getElementById('mapLayerLabel')) document.getElementById('mapLayerLabel').textContent = 'Street';
                
                vehicleMarker = L.marker([vehiclePosition.lat, vehiclePosition.lng], {
                    icon: L.divIcon({
                        className: 'vehicle-marker',
                        html: '<div style="width: 20px; height: 20px; background-color: #ef4444; border-radius: 50%; border: 2px solid white;"></div>',
                        iconSize: [20, 20]
                    })
                }).addTo(map);
                
                map.on('click', function(e) {
                    addWaypoint(e.latlng.lat, e.latlng.lng);
                });
            }
        }
        function setMapLayer(layer) {
            if (!map || !tileLayerStreet || !tileLayerSatellite) return;
            map.removeLayer(tileLayerStreet);
            map.removeLayer(tileLayerSatellite);
            if (layer === 'satellite') {
                tileLayerSatellite.addTo(map);
                currentTileLayer = 'satellite';
                if (document.getElementById('mapLayerLabel')) document.getElementById('mapLayerLabel').textContent = 'Satellite';
            } else {
                tileLayerStreet.addTo(map);
                currentTileLayer = 'street';
                if (document.getElementById('mapLayerLabel')) document.getElementById('mapLayerLabel').textContent = 'Street';
            }
        }
        let lastTrackTime = 0;
        function addTrackPoint(lat, lng) {
            if (Date.now() - lastTrackTime < 3000) return;
            lastTrackTime = Date.now();
            trackPoints.push([lat, lng]);
            if (!map || !isOnline) return;
            if (pathPolyline) map.removeLayer(pathPolyline);
            pathPolyline = L.polyline(trackPoints, { color: '#22c55e', weight: 4 }).addTo(map);
        }
        function drawWaypointPolygon() {
            if (waypointPolyline && map) map.removeLayer(waypointPolyline);
            if (waypoints.length < 2) return;
            const pts = waypoints.filter(w => w.lat).map(w => [w.lat, w.lng]);
            if (pts.length < 2) return;
            waypointPolyline = L.polyline(pts, { color: '#f59e0b', weight: 3, dashArray: '5,5' }).addTo(map);
        }
        
        function initOfflineGrid() {
            const grid = offlineGrid;
            grid.innerHTML = '';
            
            const width = grid.offsetWidth;
            const height = grid.offsetHeight;
            
            // Create grid lines
            const gridSpacing = 50; // pixels between lines
            
            for (let x = 0; x <= width; x += gridSpacing) {
                const verticalLine = document.createElement('div');
                verticalLine.className = 'grid-line vertical';
                verticalLine.style.left = `${x}px`;
                grid.appendChild(verticalLine);
                
                // Add coordinate label
                if (x > 0) {
                    const coord = document.createElement('div');
                    coord.className = 'grid-coordinates';
                    coord.style.left = `${x}px`;
                    coord.style.top = '10px';
                    coord.textContent = `${(x / gridSpacing) * gridScale}m`;
                    grid.appendChild(coord);
                }
            }
            
            for (let y = 0; y <= height; y += gridSpacing) {
                const horizontalLine = document.createElement('div');
                horizontalLine.className = 'grid-line horizontal';
                horizontalLine.style.top = `${y}px`;
                grid.appendChild(horizontalLine);
                
                // Add coordinate label
                if (y > 0) {
                    const coord = document.createElement('div');
                    coord.className = 'grid-coordinates';
                    coord.style.left = '10px';
                    coord.style.top = `${y}px`;
                    coord.textContent = `${(y / gridSpacing) * gridScale}m`;
                    grid.appendChild(coord);
                }
            }
            
            // Add vehicle marker
            const vehicle = document.createElement('div');
            vehicle.className = 'vehicle-marker';
            vehicle.id = 'gridVehicle';
            vehicle.style.left = '50%';
            vehicle.style.top = '50%';
            vehicle.style.setProperty('--heading', `${vehicleHeading}deg`);
            grid.appendChild(vehicle);
            
            // Add click event for waypoints
            grid.addEventListener('click', function(e) {
                const rect = grid.getBoundingClientRect();
                const x = e.clientX - rect.left;
                const y = e.clientY - rect.top;
                addGridWaypoint(x, y);
            });
        }
        
        function updateOfflineGridPosition() {
            const vehicle = document.getElementById('gridVehicle');
            if (vehicle) {
                // In a real implementation, this would update based on odometry/GPS
                vehicle.style.left = '50%';
                vehicle.style.top = '50%';
                vehicle.style.setProperty('--heading', `${vehicleHeading}deg`);
            }
        }
        
        function switchToOnlineMap() {
            onlineMap.style.display = 'block';
            offlineGrid.style.display = 'none';
            mapTitle.textContent = 'Online Map (Leaflet)';
            mapModeIndicator.textContent = 'Online Map';
            
            if (!map) {
                initOnlineMap();
            } else {
                map.invalidateSize();
            }
        }
        
        function switchToOfflineGrid() {
            onlineMap.style.display = 'none';
            offlineGrid.style.display = 'block';
            mapTitle.textContent = 'Offline Grid (Cartesian)';
            mapModeIndicator.textContent = 'Offline Grid';
            
            initOfflineGrid();
        }
        
        toggleMapModeBtn.addEventListener('click', function() {
            if (isOnline) {
                isOnline = false;
                switchToOfflineGrid();
                showToast('Switched to Offline Grid mode', 'warning');
            } else {
                checkInternetConnectivity();
            }
        });
        
        // Waypoint: penomoran Waypoint 1, 2... garis antar waypoint (polygon)
        function addWaypoint(lat, lng) {
            const marker = L.marker([lat, lng], {
                icon: L.divIcon({
                    className: 'waypoint-marker',
                    html: `<div style="width: 15px; height: 15px; background-color: #f59e0b; border-radius: 50%; border: 2px solid white;"></div>`,
                    iconSize: [15, 15]
                })
            }).addTo(map);
            const num = waypoints.length + 1;
            waypoints.push({ lat: lat, lng: lng, marker: marker });
            drawWaypointPolygon();
            updateWaypointListUI();
            showToast('Waypoint ' + num + ' ditambah');
            marker.on('click', function(e) {
                removeWaypoint(marker);
            });
        }
        function updateWaypointListUI() {
            var ul = document.getElementById('waypointList');
            var validationEl = document.getElementById('waypointValidation');
            if (!ul) return;
            if (waypoints.length === 0) {
                ul.innerHTML = '<li style="opacity:0.8;color:var(--text-muted);">Belum ada waypoint — klik di peta untuk menambah</li>';
                if (validationEl) { validationEl.className = 'waypoint-validation valid'; validationEl.textContent = 'Klik di peta untuk menambah waypoint. Minimal 3 titik agar jalur tertutup (polygon).'; }
                return;
            }
            ul.innerHTML = '';
            waypoints.forEach(function(wp, i) {
                var li = document.createElement('li');
                var name = 'Waypoint ' + (i + 1);
                var coord = wp.lat != null ? (wp.lat.toFixed(4) + ', ' + wp.lng.toFixed(4)) : ('(' + wp.x + ', ' + wp.y + ')');
                li.innerHTML = '<span class="waypoint-num">' + name + '</span><span class="waypoint-coords">' + coord + '</span>';
                ul.appendChild(li);
            });
            if (validationEl) {
                if (waypoints.length >= 3) {
                    validationEl.className = 'waypoint-validation valid';
                    validationEl.textContent = 'Jalur tertutup (polygon). Total ' + waypoints.length + ' waypoint.';
                } else {
                    validationEl.className = 'waypoint-validation warning';
                    validationEl.textContent = 'Tambahkan minimal 3 waypoint untuk polygon lahan. Saat ini: ' + waypoints.length + ' titik.';
                }
            }
        }
        function removeWaypoint(marker) {
            const idx = waypoints.findIndex(wp => wp.marker === marker || (wp.lat === marker.getLatLng().lat && wp.lng === marker.getLatLng().lng));
            if (idx !== -1) {
                waypoints.splice(idx, 1);
                if (map) map.removeLayer(marker);
                drawWaypointPolygon();
                updateWaypointListUI();
                showToast('Waypoint dihapus', 'warning');
            }
        }
        
        function addGridWaypoint(x, y) {
            const waypoint = document.createElement('div');
            waypoint.className = 'waypoint-marker';
            waypoint.style.left = `${x}px`;
            waypoint.style.top = `${y}px`;
            waypoint.title = 'Click to remove';
            
            offlineGrid.appendChild(waypoint);
            
            const index = waypoints.length;
            waypoints.push({x: x, y: y});
            updateWaypointListUI();
            showToast('Waypoint ' + (index + 1) + ' ditambah');
            
            waypoint.addEventListener('click', function(e) {
                e.stopPropagation();
                this.remove();
                const wpIndex = waypoints.findIndex(wp => wp.x === x && wp.y === y);
                if (wpIndex !== -1) {
                    waypoints.splice(wpIndex, 1);
                    updateWaypointListUI();
                    showToast('Waypoint dihapus', 'warning');
                }
            });
        }
        
        // Map Controls
        centerMapBtn.addEventListener('click', function() {
            if (isOnline && map) {
                map.setView(vehiclePosition, 18);
            }
            showToast('Map centered');
        });
        
        function doClearWaypoints() {
            waypoints = [];
            if (isOnline && map) {
                map.eachLayer(function(layer) {
                    if (layer instanceof L.Marker && layer !== vehicleMarker) map.removeLayer(layer);
                });
                if (waypointPolyline) { map.removeLayer(waypointPolyline); waypointPolyline = null; }
            } else {
                const gridWaypoints = offlineGrid.querySelectorAll('.waypoint-marker');
                gridWaypoints.forEach(wp => wp.remove());
            }
            updateWaypointListUI();
            showToast('Waypoint dihapus semua', 'warning');
        }
        clearWaypointsBtn.addEventListener('click', function() {
            if (confirm('Hapus semua waypoint?')) doClearWaypoints();
        });
        const clearWaypointsListBtn = document.getElementById('clearWaypointsList');
        if (clearWaypointsListBtn) clearWaypointsListBtn.addEventListener('click', function() {
            if (confirm('Hapus semua waypoint?')) doClearWaypoints();
        });
        document.getElementById('mapLayerStreet').addEventListener('click', function() { setMapLayer('street'); });
        document.getElementById('mapLayerSatellite').addEventListener('click', function() { setMapLayer('satellite'); });
        
        startNavigationBtn.addEventListener('click', function() {
            if (waypoints.length === 0) {
                showToast('Add waypoints first', 'error');
                return;
            }
            
            if (!navigationActive) {
                navigationActive = true;
                this.innerHTML = '<i class="fas fa-stop"></i> Stop Nav';
                this.classList.remove('btn-success');
                this.classList.add('btn-danger');
                showToast('Navigation started to ' + waypoints.length + ' waypoints');
                setMode('auto');
                
                // Start navigation simulation
                simulateNavigation();
            } else {
                navigationActive = false;
                this.innerHTML = '<i class="fas fa-play"></i> Start Nav';
                this.classList.remove('btn-danger');
                this.classList.add('btn-success');
                showToast('Navigation stopped');
            }
        });
        
        function simulateNavigation() {
            if (!navigationActive) return;
            
            // Simulate movement
            if (Math.random() > 0.7) {
                const speed = (Math.random() * 5).toFixed(1);
                speedValue.textContent = speed;
                
                // Update distance
                totalDistance += parseFloat(speed) * 0.2778; // Convert km/h to m/s
                
                // Log movement
                logMovement(speed);
            }
            
            // Update battery
            batteryLevel = Math.max(0, batteryLevel - 0.01);
            updateBatteryDisplay();
            
            // Continue simulation
            setTimeout(simulateNavigation, 1000);
        }
        
        // Control Buttons
        forwardControlBtn.addEventListener('click', function() {
            throttle = Math.min(100, throttle + 10);
            throttleValue.textContent = throttle + '%';
            showToast('Throttle increased to ' + throttle + '%');
        });
        
        backwardControlBtn.addEventListener('click', function() {
            throttle = Math.max(-100, throttle - 10);
            throttleValue.textContent = throttle + '%';
            showToast('Throttle decreased to ' + throttle + '%');
        });
        
        emergencyStopBtn.addEventListener('click', function() {
            if (confirm('EMERGENCY STOP - Semua motor akan berhenti segera! Lanjutkan?')) {
                throttle = 0;
                steering = 0;
                throttleValue.textContent = '0%';
                steeringValue.textContent = '0%';
                
                if (navigationActive) {
                    navigationActive = false;
                    startNavigationBtn.innerHTML = '<i class="fas fa-play"></i> Start Nav';
                    startNavigationBtn.classList.remove('btn-danger');
                    startNavigationBtn.classList.add('btn-success');
                }
                
                showToast('EMERGENCY STOP ACTIVATED!', 'error');
                logEmergencyStop();
            }
        });
        
        // Time Sync
        syncTimeBtn.addEventListener('click', function() {
            showToast('Forcing GPS time sync...');
            updateTime();
        });
        
        // Usage Log Management
        function logModeChange(mode) {
            const timestamp = getCurrentTimestamp();
            const entry = {
                timestamp: timestamp,
                mode: mode,
                distance: totalDistance.toFixed(1),
                speed: speedValue.textContent,
                battery: batteryLevel
            };
            
            logEntries.unshift(entry); // Add to beginning
            
            // Update table
            updateLogTable();
            
            // Keep only last 50 entries
            if (logEntries.length > 50) {
                logEntries.pop();
            }
            
            // Save to localStorage
            localStorage.setItem('aiTracLog', JSON.stringify(logEntries));
        }
        
        function logMovement(speed) {
            const timestamp = getCurrentTimestamp();
            const entry = {
                timestamp: timestamp,
                mode: currentMode,
                distance: totalDistance.toFixed(1),
                speed: speed,
                battery: batteryLevel
            };
            
            // Add to log if significant movement
            if (parseFloat(speed) > 0.5) {
                logEntries.unshift(entry);
                
                if (logEntries.length > 50) {
                    logEntries.pop();
                }
                
                updateLogTable();
                localStorage.setItem('aiTracLog', JSON.stringify(logEntries));
            }
        }
        
        function logEmergencyStop() {
            const timestamp = getCurrentTimestamp();
            const entry = {
                timestamp: timestamp,
                mode: 'EMERGENCY',
                distance: totalDistance.toFixed(1),
                speed: '0.0',
                battery: batteryLevel,
                emergency: true
            };
            
            logEntries.unshift(entry);
            updateLogTable();
            localStorage.setItem('aiTracLog', JSON.stringify(logEntries));
        }
        
        function updateLogTable() {
            logTableBody.innerHTML = '';
            
            logEntries.forEach(entry => {
                const row = document.createElement('tr');
                
                // Determine battery class
                let batteryClass = 'battery-high';
                if (entry.battery < 20) batteryClass = 'battery-low';
                else if (entry.battery < 50) batteryClass = 'battery-medium';
                
                // Emergency row styling
                if (entry.emergency) {
                    row.style.backgroundColor = 'rgba(244, 67, 54, 0.1)';
                }
                
                row.innerHTML = `
                    <td class="log-timestamp">${entry.timestamp}</td>
                    <td><span class="log-mode ${entry.mode.toLowerCase()}">${entry.mode.toUpperCase()}</span></td>
                    <td class="log-distance">${entry.distance}</td>
                    <td class="log-distance">${entry.speed}</td>
                    <td class="log-battery">
                        <div class="battery-indicator">
                            <div class="battery-level">
                                <div class="battery-fill ${batteryClass}" style="width: ${entry.battery}%"></div>
                            </div>
                            <span class="battery-percentage">${Math.round(entry.battery)}%</span>
                        </div>
                    </td>
                `;
                
                logTableBody.appendChild(row);
            });
        }
        
        function getCurrentTimestamp() {
            const now = new Date();
            const gpsTime = localStorage.getItem('lastGPSTime');
            
            if (gpsTime) {
                return gpsTime;
            }
            
            // Fallback to system time
            return now.toISOString().replace('T', ' ').substring(0, 19);
        }
        
        function updateBatteryDisplay() {
            batteryLevel = Math.max(0, Math.min(100, batteryLevel));
            
            let batteryClass = 'battery-high';
            if (batteryLevel < 20) batteryClass = 'battery-low';
            else if (batteryLevel < 50) batteryClass = 'battery-medium';
            
            batteryFill.className = `battery-fill ${batteryClass}`;
            batteryFill.style.width = `${batteryLevel}%`;
            batteryPercentage.textContent = `${Math.round(batteryLevel)}%`;
            
            // Low battery warning
            if (batteryLevel < 15 && batteryLevel > 0) {
                showToast(`Low battery: ${Math.round(batteryLevel)}% remaining`, 'warning');
            } else if (batteryLevel === 0) {
                showToast('Battery critically low!', 'error');
            }
        }
        
        clearLogBtn.addEventListener('click', function() {
            if (confirm('Clear all log entries?')) {
                logEntries = [];
                updateLogTable();
                localStorage.removeItem('aiTracLog');
                showToast('Log cleared', 'warning');
            }
        });
        
        exportLogBtn.addEventListener('click', function() {
            if (logEntries.length === 0) {
                showToast('Tidak ada log untuk export', 'warning');
                return;
            }
            let csv = 'Waktu,Mode,Jarak(m),Kecepatan(km/h),Baterai(%)\n';
            logEntries.forEach(entry => {
                csv += `"${entry.timestamp}","${entry.mode}",${entry.distance},${entry.speed},${Math.round(entry.battery)}\n`;
            });
            const blob = new Blob([csv], { type: 'text/csv' });
            const url = window.URL.createObjectURL(blob);
            const a = document.createElement('a');
            a.href = url;
            a.download = `traktor-log-${new Date().toISOString().slice(0,10)}.csv`;
            document.body.appendChild(a);
            a.click();
            document.body.removeChild(a);
            window.URL.revokeObjectURL(url);
            showToast('Log diexport CSV');
        });

        // History: List / Grafik / Replay
        document.querySelectorAll('.history-tab').forEach(tab => {
            tab.addEventListener('click', function() {
                const view = this.getAttribute('data-view');
                document.querySelectorAll('.history-tab').forEach(t => t.classList.remove('active'));
                document.querySelectorAll('.history-view').forEach(v => v.classList.remove('active'));
                this.classList.add('active');
                let panel = document.getElementById('historyList');
                if (view === 'graph') panel = document.getElementById('historyGraph');
                if (view === 'replay') panel = document.getElementById('historyReplay');
                if (panel) panel.classList.add('active');
                if (view === 'graph') drawLogChart();
                if (view === 'replay') initReplay();
            });
        });
        function drawLogChart() {
            const canvas = document.getElementById('logChart');
            if (!canvas || logEntries.length === 0) return;
            const ctx = canvas.getContext('2d');
            const w = canvas.width;
            const h = canvas.height;
            ctx.clearRect(0, 0, w, h);
            const n = Math.min(logEntries.length, 20);
            const dist = logEntries.slice(0, n).map(e => parseFloat(e.distance) || 0);
            const maxD = Math.max(1, Math.max.apply(null, dist));
            const barW = (w - 40) / n - 2;
            for (let i = 0; i < n; i++) {
                const x = 20 + i * (barW + 2);
                const bh = (dist[n - 1 - i] / maxD) * (h - 30);
                ctx.fillStyle = 'var(--primary)';
                ctx.fillRect(x, h - 20 - bh, barW, bh);
            }
            ctx.fillStyle = 'var(--text-color)';
            ctx.font = '10px sans-serif';
            ctx.fillText('Jarak (m) - ' + n + ' entri terakhir', 20, 12);
        }
        let replayInterval = null;
        function initReplay() {
            const pts = trackPoints.length ? trackPoints : [];
            const hint = document.getElementById('replayHint');
            const slider = document.getElementById('replaySlider');
            const timeEl = document.getElementById('replayTime');
            if (pts.length < 2) {
                if (hint) hint.textContent = 'Jalankan operasi dulu agar ada data lintasan untuk replay.';
                if (slider) slider.max = 0;
                return;
            }
            if (hint) hint.textContent = pts.length + ' titik lintasan. Klik Putar untuk replay.';
            slider.max = pts.length - 1;
            slider.value = 0;
            timeEl.textContent = '0:00';
        }
        document.getElementById('replayStart').addEventListener('click', function() {
            const pts = trackPoints;
            const slider = document.getElementById('replaySlider');
            const timeEl = document.getElementById('replayTime');
            if (pts.length < 2 || !map || !vehicleMarker) return;
            if (replayInterval) {
                clearInterval(replayInterval);
                replayInterval = null;
                this.innerHTML = '<i class="fas fa-play"></i> Putar';
                return;
            }
            this.innerHTML = '<i class="fas fa-stop"></i> Stop';
            let idx = 0;
            replayInterval = setInterval(() => {
                if (idx >= pts.length) {
                    clearInterval(replayInterval);
                    replayInterval = null;
                    document.getElementById('replayStart').innerHTML = '<i class="fas fa-play"></i> Putar';
                    return;
                }
                vehicleMarker.setLatLng([pts[idx][0], pts[idx][1]]);
                slider.value = idx;
                timeEl.textContent = Math.floor(idx / 10) + ':' + (idx % 10);
                idx++;
            }, 200);
        });
        document.getElementById('replaySlider').addEventListener('input', function() {
            const pts = trackPoints;
            const idx = parseInt(this.value, 10);
            if (pts.length && map && vehicleMarker && idx >= 0 && idx < pts.length) {
                vehicleMarker.setLatLng([pts[idx][0], pts[idx][1]]);
            }
        });
        
        // Kirim mode ke ESP32
        function sendModeCommand(mode) {
            fetch('/setMode?mode=' + encodeURIComponent(mode))
                .then(r => r.text())
                .then(() => console.log('Mode:', mode))
                .catch(console.error);
        }
        
        // Toast Notification
        function showToast(message, type) {
            if (!type) type = 'success';
            
            toastMessage.textContent = message;
            toast.className = 'toast';
            
            if (type === 'error') {
                toast.classList.add('error');
                toast.querySelector('i').className = 'fas fa-exclamation-circle';
            } else if (type === 'warning') {
                toast.classList.add('warning');
                toast.querySelector('i').className = 'fas fa-exclamation-triangle';
            } else {
                toast.querySelector('i').className = 'fas fa-check-circle';
            }
            
            setTimeout(function() {
                toast.classList.add('show');
            }, 10);
            
            setTimeout(function() {
                toast.classList.remove('show');
            }, 3000);
        }
        
        // Sidebar navigasi: scroll ke section & set active
        document.querySelectorAll('.sidebar-nav .nav-link').forEach(function(link) {
            link.addEventListener('click', function(e) {
                e.preventDefault();
                var sectionId = this.getAttribute('data-section');
                document.querySelectorAll('.sidebar-nav .nav-link').forEach(function(l) { l.classList.remove('active'); });
                this.classList.add('active');
                var el = document.getElementById(sectionId);
                if (el) el.scrollIntoView({ behavior: 'smooth', block: 'start' });
                var sidebar = document.getElementById('sidebar');
                if (sidebar && window.innerWidth <= 1024) sidebar.classList.remove('open');
            });
        });
        // Menu toggle (mobile): buka/tutup sidebar
        var menuToggle = document.getElementById('menuToggle');
        var sidebar = document.getElementById('sidebar');
        var sidebarOverlay = document.getElementById('sidebarOverlay');
        if (menuToggle && sidebar) {
            menuToggle.addEventListener('click', function() {
                sidebar.classList.toggle('open');
                if (sidebarOverlay) sidebarOverlay.classList.toggle('show', sidebar.classList.contains('open'));
            });
        }
        if (sidebarOverlay) {
            sidebarOverlay.addEventListener('click', function() {
                if (sidebar) { sidebar.classList.remove('open'); sidebarOverlay.classList.remove('show'); }
            });
        }
        document.addEventListener('click', function(e) {
            if (sidebar && sidebar.classList.contains('open') && window.innerWidth <= 1024) {
                if (!sidebar.contains(e.target) && menuToggle && !menuToggle.contains(e.target)) {
                    sidebar.classList.remove('open');
                    if (sidebarOverlay) sidebarOverlay.classList.remove('show');
                }
            }
        });
        // Pause / Stop navigation
        var pauseBtn = document.getElementById('pauseNavigation');
        var stopBtn = document.getElementById('stopNavigation');
        if (pauseBtn) pauseBtn.addEventListener('click', function() {
            if (navigationActive) {
                navigationActive = false;
                var startBtn = document.getElementById('startNavigation');
                if (startBtn) { startBtn.innerHTML = '<i class="fas fa-play"></i> Start'; startBtn.classList.remove('btn-danger'); startBtn.classList.add('btn-success'); }
                showToast('Navigasi dijeda', 'warning');
            }
        });
        if (stopBtn) stopBtn.addEventListener('click', function() {
            if (navigationActive) {
                navigationActive = false;
                var startBtn = document.getElementById('startNavigation');
                if (startBtn) { startBtn.innerHTML = '<i class="fas fa-play"></i> Start'; startBtn.classList.remove('btn-danger'); startBtn.classList.add('btn-success'); }
                showToast('Navigasi dihentikan', 'warning');
            }
        });
        // Filter History by mode & date
        var filterModeEl = document.getElementById('filterMode');
        var filterDateEl = document.getElementById('filterDate');
        function applyHistoryFilter() {
            var mode = filterModeEl ? filterModeEl.value : '';
            var date = filterDateEl ? filterDateEl.value : '';
            var filtered = logEntries.filter(function(e) {
                if (mode && (e.mode || '').toLowerCase() !== mode) return false;
                if (date && (e.timestamp || '').indexOf(date) === -1) return false;
                return true;
            });
            updateLogTableFiltered(filtered);
        }
        function updateLogTableFiltered(entries) {
            if (!logTableBody) return;
            logTableBody.innerHTML = '';
            (entries || logEntries).forEach(function(entry) {
                var batteryClass = entry.battery < 20 ? 'battery-low' : (entry.battery < 50 ? 'battery-medium' : 'battery-high');
                var row = document.createElement('tr');
                if (entry.emergency) row.style.backgroundColor = 'rgba(239, 68, 68, 0.1)';
                row.innerHTML = '<td class="log-timestamp">' + entry.timestamp + '</td><td><span class="log-mode ' + (entry.mode || '').toLowerCase() + '">' + (entry.mode || '').toUpperCase() + '</span></td><td class="log-distance">' + entry.distance + '</td><td class="log-distance">' + entry.speed + '</td><td class="log-battery"><div class="battery-indicator"><div class="battery-level"><div class="battery-fill ' + batteryClass + '" style="width:' + entry.battery + '%"></div></div><span class="battery-percentage">' + Math.round(entry.battery) + '%</span></div></td>';
                logTableBody.appendChild(row);
            });
        }
        if (filterModeEl) filterModeEl.addEventListener('change', applyHistoryFilter);
        if (filterDateEl) filterDateEl.addEventListener('change', applyHistoryFilter);

        // Virtual Joystick - Manual Mode
        var joystickContainer = document.getElementById('joystickContainer');
        var joystickStick = document.getElementById('joystickStick');
        if (joystickContainer && joystickStick) {
            var jcRect = function() { return joystickContainer.getBoundingClientRect(); };
            var maxDist = 35;
            function onJoystickMove(clientX, clientY) {
                var rect = jcRect();
                var cx = rect.left + rect.width / 2;
                var cy = rect.top + rect.height / 2;
                var dx = clientX - cx;
                var dy = clientY - cy;
                var dist = Math.min(Math.sqrt(dx*dx + dy*dy), maxDist);
                var angle = Math.atan2(dy, dx);
                var sx = Math.cos(angle) * dist;
                var sy = Math.sin(angle) * dist;
                joystickStick.style.transform = 'translate(calc(-50% + ' + sx + 'px), calc(-50% + ' + sy + 'px))';
                var steer = Math.round((sx / maxDist) * 100);
                var throt = Math.round((-sy / maxDist) * 100);
                throttle = Math.max(-100, Math.min(100, throt));
                steering = Math.max(-100, Math.min(100, steer));
                if (throttleValue) throttleValue.textContent = throttle + '%';
                if (steeringValue) steeringValue.textContent = steering + '%';
                fetch('/setSteering?value=' + steering).catch(function(){});
            }
            function onJoystickEnd() {
                joystickStick.style.transform = 'translate(-50%, -50%)';
                throttle = 0;
                steering = 0;
                if (throttleValue) throttleValue.textContent = '0%';
                if (steeringValue) steeringValue.textContent = '0%';
                document.removeEventListener('mousemove', onMouseMove);
                document.removeEventListener('mouseup', onJoystickEnd);
                document.removeEventListener('touchmove', onTouchMove);
                document.removeEventListener('touchend', onJoystickEnd);
            }
            function onMouseMove(e) { onJoystickMove(e.clientX, e.clientY); }
            function onTouchMove(e) { if (e.touches.length) onJoystickMove(e.touches[0].clientX, e.touches[0].clientY); }
            joystickStick.addEventListener('mousedown', function(e) {
                e.preventDefault();
                document.addEventListener('mousemove', onMouseMove);
                document.addEventListener('mouseup', onJoystickEnd);
            });
            joystickStick.addEventListener('touchstart', function(e) {
                e.preventDefault();
                document.addEventListener('touchmove', onTouchMove);
                document.addEventListener('touchend', onJoystickEnd);
            });
        }

        // Settings tabs
        document.querySelectorAll('.settings-tab').forEach(function(tab) {
            tab.addEventListener('click', function() {
                var id = this.getAttribute('data-settings');
                document.querySelectorAll('.settings-tab').forEach(function(t) { t.classList.remove('active'); });
                document.querySelectorAll('.settings-section').forEach(function(s) { s.classList.remove('active'); });
                this.classList.add('active');
                var section = document.getElementById('settings' + (id === 'aiml' ? 'Aiml' : id === 'ui' ? 'Ui' : id.charAt(0).toUpperCase() + id.slice(1)));
                if (section) section.classList.add('active');
            });
        });
        var themeToggleSettings = document.getElementById('themeToggleSettings');
        if (themeToggleSettings) themeToggleSettings.addEventListener('click', function() { themeToggle && themeToggle.click(); });
        document.getElementById('settingsScanWifi') && document.getElementById('settingsScanWifi').addEventListener('click', function() { scanWifiBtn && scanWifiBtn.click(); });

        // Initialize
        document.addEventListener('DOMContentLoaded', function() {
            initTheme();
            
            // Load saved log entries
            const savedLog = localStorage.getItem('aiTracLog');
            if (savedLog) {
                try {
                    logEntries = JSON.parse(savedLog);
                    updateLogTable();
                } catch (e) {
                    console.error('Error loading log:', e);
                }
            }
            
            // Load connected WiFi
            const connectedWiFi = localStorage.getItem('connectedWiFiSSID');
            if (connectedWiFi) {
                connectedSSID.textContent = connectedWiFi;
                wifiStatus.textContent = 'Connected (STA)';
            }
            
            // Check initial connectivity
            checkInternetConnectivity();
            
            // Initialize battery display
            updateBatteryDisplay();
            
            // Simulate initial sensor data
            setInterval(() => {
                // Update ultrasonic sensors
                const front = Math.floor(Math.random() * 200) + 10;
                const left = Math.floor(Math.random() * 150) + 5;
                const right = Math.floor(Math.random() * 150) + 5;
                ultrasonicValue.textContent = `${front}/${left}/${right}`;
                
                // Update heading
                if (navigationActive) {
                    headingValue.textContent = vehicleHeading = (vehicleHeading + Math.random() * 10 - 5) % 360;
                }
            }, 2000);
            
            showToast('Dashboard Traktor Otonom siap - IoT & GPS Real-time');
        });
    </script>
</body>
</html>

)rawliteral";

#endif
