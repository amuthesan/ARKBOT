#!/usr/bin/env python3
"""
ARK-BOT Companion GUI - Accurate 3D Kinematics Viewport
Renders the robot chassis, 4 articulated legs (Coxa, Femur, Tibia), joint spheres,
and ground contact pads using exact firmware trigonometric kinematics.
"""

import math
from PySide6.QtWidgets import QWidget
from PySide6.QtCore import Qt, QPointF, QRectF
from PySide6.QtGui import (
    QPainter, QPen, QBrush, QColor, QFont,
    QLinearGradient, QRadialGradient, QPolygonF
)

# Physical Kinematics Constants from firmware config.h (mm)
LENGTH_A    = 84.0   # Femur (Thigh) length
LENGTH_B    = 145.0  # Tibia (Calf) length
LENGTH_C    = 72.5   # Coxa (Hip) length
LENGTH_SIDE = 145.4  # Body chassis base width/length
CHASSIS_H   = 18.0   # Chassis height

# Leg Mount Base Coordinates on Chassis (X: Forward, Y: Right, Z: Up)
LEG_MOUNTS = [
    ( LENGTH_SIDE / 2.0,  LENGTH_SIDE / 2.0, 0.0),  # Leg 0: Front-Right (FR)
    (-LENGTH_SIDE / 2.0,  LENGTH_SIDE / 2.0, 0.0),  # Leg 1: Rear-Right (RR)
    ( LENGTH_SIDE / 2.0, -LENGTH_SIDE / 2.0, 0.0),  # Leg 2: Front-Left (FL)
    (-LENGTH_SIDE / 2.0, -LENGTH_SIDE / 2.0, 0.0),  # Leg 3: Rear-Left (RL)
]

LEG_NAMES = ["FR (Front-Right)", "RR (Rear-Right)", "FL (Front-Left)", "RL (Rear-Left)"]
LEG_COLORS = [
    QColor(0, 240, 255),   # Leg 0: Cyan
    QColor(168, 85, 247),  # Leg 1: Purple
    QColor(16, 185, 129),  # Leg 2: Emerald
    QColor(245, 158, 11),  # Leg 3: Amber
]


class Viewport3D(QWidget):
    """
    High-performance 3D vector graphics widget with real-time kinematics visualization.
    """
    def __init__(self, parent=None):
        super().__init__(parent)
        self.setMinimumSize(480, 360)
        self.setFocusPolicy(Qt.StrongFocus)

        # Camera & View Transform
        self.yaw = 40.0       # Rotation around Z axis (deg)
        self.pitch = 24.0     # Tilt angle from horizon (deg)
        self.zoom = 1.05      # Scale factor
        self.pan_x = 0.0      # Pan offset X
        self.pan_y = 15.0     # Pan offset Y

        # Mouse Interaction State
        self.last_mouse_pos = None
        self.is_rotating = False
        self.is_panning = False

        # Live Robot State (Current & Target for Smooth Animation LERP)
        self.sites = [
            [124.0, 0.0, -100.0],  # Leg 0: FR
            [124.0, 0.0, -100.0],  # Leg 1: RR
            [124.0, 0.0, -100.0],  # Leg 2: FL
            [124.0, 0.0, -100.0],  # Leg 3: RL
        ]
        self.display_sites = [list(s) for s in self.sites]

        self.angles = [
            [90, 90, 90],
            [90, 90, 90],
            [90, 90, 90],
            [90, 90, 90]
        ]
        self.moving = False
        self.pca_ready = True
        self.robot_mode = "STAND"
        self.telemetry_active = False

        # IMU 6-Axis Attitude
        self.imu_ready = False
        self.imu_pitch = 0.0
        self.imu_roll = 0.0
        self.display_pitch = 0.0
        self.display_roll = 0.0

    def _rotate_body_point(self, x, y, z):
        """Rotates a 3D point using current IMU pitch and roll."""
        if not self.imu_ready:
            return x, y, z
        phi = math.radians(self.display_roll)
        theta = math.radians(self.display_pitch)
        rx = x * math.cos(phi) - z * math.sin(phi)
        ry = y * math.cos(theta) - z * math.sin(theta)
        rz = z + x * math.sin(phi) + y * math.sin(theta)
        return rx, ry, rz

    def update_telemetry(self, telem):
        """Update robot state from telemetry packet and trigger repaint."""
        if not telem:
            return

        self.telemetry_active = True

        if "sites" in telem and isinstance(telem["sites"], list) and len(telem["sites"]) == 4:
            for i in range(4):
                if isinstance(telem["sites"][i], list) and len(telem["sites"][i]) == 3:
                    self.sites[i] = [
                        float(telem["sites"][i][0]),
                        float(telem["sites"][i][1]),
                        float(telem["sites"][i][2])
                    ]

        if "angles" in telem and isinstance(telem["angles"], list) and len(telem["angles"]) == 4:
            self.angles = telem["angles"]

        if "mode" in telem:
            self.robot_mode = str(telem["mode"])

        if "moving" in telem:
            self.moving = bool(telem["moving"])

        if "pca" in telem:
            self.pca_ready = bool(telem["pca"])

        if "imu" in telem and isinstance(telem["imu"], dict):
            self.imu_ready = bool(telem["imu"].get("ready", False))
            self.imu_pitch = float(telem["imu"].get("pitch", 0.0))
            self.imu_roll  = float(telem["imu"].get("roll", 0.0))

        self.update()

    def advance_animation(self):
        """Smooth LERP interpolation for fluid 60 FPS motion."""
        changed = False
        lerp_factor = 0.35
        for i in range(4):
            for j in range(3):
                diff = self.sites[i][j] - self.display_sites[i][j]
                if abs(diff) > 0.1:
                    self.display_sites[i][j] += diff * lerp_factor
                    changed = True
                else:
                    self.display_sites[i][j] = self.sites[i][j]

        # Smooth IMU Pitch & Roll LERP
        diff_p = self.imu_pitch - self.display_pitch
        if abs(diff_p) > 0.05:
            self.display_pitch += diff_p * 0.3
            changed = True
        else:
            self.display_pitch = self.imu_pitch

        diff_r = self.imu_roll - self.display_roll
        if abs(diff_r) > 0.05:
            self.display_roll += diff_r * 0.3
            changed = True
        else:
            self.display_roll = self.imu_roll

        if changed:
            self.update()

    def reset_view(self):
        self.yaw = 40.0
        self.pitch = 24.0
        self.zoom = 1.05
        self.pan_x = 0.0
        self.pan_y = 15.0
        self.update()

    # --- Mouse & Interaction Events ---
    def mousePressEvent(self, event):
        self.last_mouse_pos = event.position()
        if event.button() == Qt.LeftButton:
            if event.modifiers() & Qt.ShiftModifier:
                self.is_panning = True
            else:
                self.is_rotating = True
        elif event.button() in (Qt.RightButton, Qt.MiddleButton):
            self.is_panning = True

    def mouseMoveEvent(self, event):
        if self.last_mouse_pos is None:
            return
        delta = event.position() - self.last_mouse_pos
        self.last_mouse_pos = event.position()

        if self.is_rotating:
            self.yaw = (self.yaw + delta.x() * 0.5) % 360.0
            self.pitch = max(5.0, min(85.0, self.pitch + delta.y() * 0.4))
            self.update()
        elif self.is_panning:
            self.pan_x += delta.x() * 0.8
            self.pan_y += delta.y() * 0.8
            self.update()

    def mouseReleaseEvent(self, event):
        self.is_rotating = False
        self.is_panning = False
        self.last_mouse_pos = None

    def wheelEvent(self, event):
        delta = event.angleDelta().y()
        zoom_factor = 1.1 if delta > 0 else 0.9
        self.zoom = max(0.3, min(3.5, self.zoom * zoom_factor))
        self.update()

    def mouseDoubleClickEvent(self, event):
        self.reset_view()

    # --- 3D Projection Math ---
    def project_3d(self, x, y, z, cx, cy):
        """
        Projects 3D world coordinate (x: forward, y: right, z: up) to 2D screen space.
        """
        rad_yaw = math.radians(self.yaw)
        rad_pitch = math.radians(self.pitch)

        # 1. Rotate Yaw around Z axis
        x1 = x * math.cos(rad_yaw) - y * math.sin(rad_yaw)
        y1 = x * math.sin(rad_yaw) + y * math.cos(rad_yaw)
        z1 = z

        # 2. Rotate Pitch around X axis
        x2 = x1
        y2 = y1 * math.cos(rad_pitch) - z1 * math.sin(rad_pitch)
        z2 = y1 * math.sin(rad_pitch) + z1 * math.cos(rad_pitch)

        # 3. Screen coordinates
        scale = 1.4 * self.zoom
        screen_x = cx + self.pan_x + x2 * scale
        screen_y = cy + self.pan_y - z2 * scale  # Invert Z for screen Y

        return (screen_x, screen_y, y2)

    # --- Exact Forward Kinematics Implementation from Firmware Formulas ---
    def compute_leg_joints(self, leg_idx):
        """
        Calculates the 4 3D joint positions for leg `leg_idx`:
        1. Base Mount on Body (Mx, My, 0)
        2. Coxa Tip / Hip Joint
        3. Knee Joint (Femur / Tibia articulation)
        4. Foot Tip (Ground Contact Pad)
        """
        mx, my, mz = LEG_MOUNTS[leg_idx]
        x, y, z = self.display_sites[leg_idx]

        # Cartesian to Polar inverse kinematics (exact firmware math)
        w = math.sqrt(x * x + y * y)
        if x < 0:
            w = -w
        v = w - LENGTH_C

        # Clamp acos arguments for numerical safety
        dist_sq = v * v + z * z
        dist = math.sqrt(max(0.001, dist_sq))

        femur_arg = (LENGTH_A * LENGTH_A - LENGTH_B * LENGTH_B + dist_sq) / (2.0 * LENGTH_A * dist)
        femur_arg = max(-1.0, min(1.0, femur_arg))

        alpha = math.atan2(z, v) + math.acos(femur_arg)
        gamma = math.atan2(y, x) if w >= 0 else math.atan2(-y, -x)

        # Unit vector along leg horizontal heading
        hyp = math.sqrt(x * x + y * y)
        if hyp < 0.001:
            hyp = 0.001

        if leg_idx in (0, 1):  # Right Legs (FR, RR)
            ux = y / hyp
            uy = x / hyp
        else:                  # Left Legs (FL, RL)
            ux = y / hyp
            uy = -x / hyp

        # 1. Base Mount (Rotated with IMU Attitude)
        p0 = self._rotate_body_point(mx, my, mz)

        # 2. Coxa Tip
        p1 = self._rotate_body_point(
            mx + LENGTH_C * ux,
            my + LENGTH_C * uy,
            mz
        )

        # 3. Knee Joint
        femur_h = LENGTH_A * math.cos(alpha)
        femur_v = LENGTH_A * math.sin(alpha)
        p2 = self._rotate_body_point(
            mx + LENGTH_C * ux + femur_h * ux,
            my + LENGTH_C * uy + femur_h * uy,
            mz + femur_v
        )

        # 4. Foot Tip
        if leg_idx in (0, 1):  # Right Legs
            p3 = (mx + y, my + x, mz + z)
        else:                  # Left Legs
            p3 = (mx + y, my - x, mz + z)

        return p0, p1, p2, p3

    # --- Paint Rendering Pipeline ---
    def paintEvent(self, event):
        painter = QPainter(self)
        painter.setRenderHint(QPainter.Antialiasing, True)
        painter.setRenderHint(QPainter.SmoothPixmapTransform, True)

        w = self.width()
        h = self.height()
        cx = w / 2.0
        cy = h / 2.0

        # 1. Cyber Dark Background
        bg_gradient = QLinearGradient(0, 0, 0, h)
        bg_gradient.setColorAt(0.0, QColor(7, 10, 15))
        bg_gradient.setColorAt(1.0, QColor(13, 19, 33))
        painter.fillRect(0, 0, w, h, bg_gradient)

        # 2. Floor Grid & World Axes
        self._draw_floor_grid(painter, cx, cy)

        # 3. Collect 3D Objects with Depth for Sorting
        primitives = []

        # Chassis Body Box
        chassis_poly_top, chassis_poly_bottom, chassis_depth = self._get_chassis_polygons(cx, cy)
        primitives.append((chassis_depth, "chassis", (chassis_poly_top, chassis_poly_bottom)))

        # 4 Articulated Legs
        for i in range(4):
            p0, p1, p2, p3 = self.compute_leg_joints(i)
            sp0 = self.project_3d(*p0, cx, cy)
            sp1 = self.project_3d(*p1, cx, cy)
            sp2 = self.project_3d(*p2, cx, cy)
            sp3 = self.project_3d(*p3, cx, cy)
            sp_shadow = self.project_3d(p3[0], p3[1], -115.0, cx, cy)

            avg_depth = (sp0[2] + sp1[2] + sp2[2] + sp3[2]) / 4.0
            primitives.append((
                avg_depth,
                "leg",
                (i, sp0, sp1, sp2, sp3, sp_shadow)
            ))

        # Sort back-to-front (smaller depth value first)
        primitives.sort(key=lambda item: item[0])

        # 4. Render 3D Objects
        for depth, obj_type, data in primitives:
            if obj_type == "chassis":
                self._draw_chassis(painter, data[0], data[1])
            elif obj_type == "leg":
                self._draw_leg(painter, *data)

        # 5. On-Screen HUD Overlay
        self._draw_hud(painter, w, h)

        painter.end()

    def _draw_floor_grid(self, painter, cx, cy):
        grid_z = -120.0
        grid_extent = 280.0
        grid_step = 40.0

        grid_pen = QPen(QColor(0, 240, 255, 30), 1.0)
        painter.setPen(grid_pen)

        # X lines (Forward/Back)
        y = -grid_extent
        while y <= grid_extent:
            p_start = self.project_3d(-grid_extent, y, grid_z, cx, cy)
            p_end = self.project_3d(grid_extent, y, grid_z, cx, cy)
            painter.drawLine(QPointF(p_start[0], p_start[1]), QPointF(p_end[0], p_end[1]))
            y += grid_step

        # Y lines (Left/Right)
        x = -grid_extent
        while x <= grid_extent:
            p_start = self.project_3d(x, -grid_extent, grid_z, cx, cy)
            p_end = self.project_3d(x, grid_extent, grid_z, cx, cy)
            painter.drawLine(QPointF(p_start[0], p_start[1]), QPointF(p_end[0], p_end[1]))
            x += grid_step

        # Coordinate Frame Origin & Forward Vector
        p_origin = self.project_3d(0, 0, grid_z, cx, cy)
        p_x = self.project_3d(70, 0, grid_z, cx, cy)   # Forward (X)
        p_y = self.project_3d(0, 70, grid_z, cx, cy)   # Right (Y)

        painter.setPen(QPen(QColor(239, 68, 68, 180), 2.0))
        painter.drawLine(QPointF(p_origin[0], p_origin[1]), QPointF(p_x[0], p_x[1]))
        painter.drawText(QPointF(p_x[0] + 5, p_x[1]), "+X (FWD)")

        painter.setPen(QPen(QColor(16, 185, 129, 180), 2.0))
        painter.drawLine(QPointF(p_origin[0], p_origin[1]), QPointF(p_y[0], p_y[1]))
        painter.drawText(QPointF(p_y[0] + 5, p_y[1]), "+Y (RIGHT)")

    def _get_chassis_polygons(self, cx, cy):
        hx = LENGTH_SIDE / 2.0
        hy = LENGTH_SIDE / 2.0
        hz = CHASSIS_H / 2.0

        top_corners = [
            self._rotate_body_point( hx,  hy,  hz),
            self._rotate_body_point(-hx,  hy,  hz),
            self._rotate_body_point(-hx, -hy,  hz),
            self._rotate_body_point( hx, -hy,  hz),
        ]
        bottom_corners = [
            self._rotate_body_point( hx,  hy, -hz),
            self._rotate_body_point(-hx,  hy, -hz),
            self._rotate_body_point(-hx, -hy, -hz),
            self._rotate_body_point( hx, -hy, -hz),
        ]

        p_top = [self.project_3d(x, y, z, cx, cy) for (x, y, z) in top_corners]
        p_bot = [self.project_3d(x, y, z, cx, cy) for (x, y, z) in bottom_corners]

        poly_top = QPolygonF([QPointF(p[0], p[1]) for p in p_top])
        poly_bot = QPolygonF([QPointF(p[0], p[1]) for p in p_bot])

        avg_depth = sum(p[2] for p in p_top + p_bot) / 8.0
        return poly_top, poly_bot, avg_depth

    def _draw_chassis(self, painter, poly_top, poly_bot):
        # Chassis Walls
        side_pen = QPen(QColor(0, 240, 255, 140), 1.5)
        side_brush = QBrush(QColor(15, 23, 42, 230))
        painter.setPen(side_pen)
        painter.setBrush(side_brush)

        for i in range(4):
            next_i = (i + 1) % 4
            side_poly = QPolygonF([
                poly_top[i], poly_top[next_i],
                poly_bot[next_i], poly_bot[i]
            ])
            painter.drawPolygon(side_poly)

        # Top Plate
        top_brush = QBrush(QColor(22, 33, 56, 245))
        painter.setBrush(top_brush)
        painter.drawPolygon(poly_top)

        # Center Ark-Bot Core Reactor
        center_x = (poly_top[0].x() + poly_top[2].x()) / 2.0
        center_y = (poly_top[0].y() + poly_top[2].y()) / 2.0

        glow = QRadialGradient(center_x, center_y, 24.0)
        glow.setColorAt(0.0, QColor(0, 240, 255, 200))
        glow.setColorAt(0.5, QColor(0, 150, 255, 100))
        glow.setColorAt(1.0, QColor(0, 240, 255, 0))
        painter.setBrush(QBrush(glow))
        painter.setPen(Qt.NoPen)
        painter.drawEllipse(QPointF(center_x, center_y), 20, 20)

        # Forward Direction Triangle
        fwd_p = (poly_top[0] + poly_top[3]) / 2.0
        arrow_pen = QPen(QColor(0, 240, 255, 255), 2.5)
        painter.setPen(arrow_pen)
        painter.drawLine(QPointF(center_x, center_y), fwd_p)

    def _draw_leg(self, painter, leg_idx, sp0, sp1, sp2, sp3, sp_shadow):
        col = LEG_COLORS[leg_idx]

        # 1. Ground Shadow under Foot
        shadow_brush = QRadialGradient(sp_shadow[0], sp_shadow[1], 20.0)
        shadow_brush.setColorAt(0.0, QColor(0, 0, 0, 160))
        shadow_brush.setColorAt(1.0, QColor(0, 0, 0, 0))
        painter.setPen(Qt.NoPen)
        painter.setBrush(QBrush(shadow_brush))
        painter.drawEllipse(QPointF(sp_shadow[0], sp_shadow[1]), 18, 9)

        # 2. Coxa Link (Base Mount -> Hip Joint)
        coxa_pen = QPen(QColor(col.red(), col.green(), col.blue(), 230), 7.0, Qt.SolidLine, Qt.RoundCap)
        painter.setPen(coxa_pen)
        painter.drawLine(QPointF(sp0[0], sp0[1]), QPointF(sp1[0], sp1[1]))

        # 3. Femur Link (Hip -> Knee Joint)
        femur_pen = QPen(QColor(col.red(), col.green(), col.blue(), 210), 5.5, Qt.SolidLine, Qt.RoundCap)
        painter.setPen(femur_pen)
        painter.drawLine(QPointF(sp1[0], sp1[1]), QPointF(sp2[0], sp2[1]))

        # 4. Tibia Link (Knee -> Foot Tip)
        tibia_pen = QPen(QColor(col.red(), col.green(), col.blue(), 190), 4.0, Qt.SolidLine, Qt.RoundCap)
        painter.setPen(tibia_pen)
        painter.drawLine(QPointF(sp2[0], sp2[1]), QPointF(sp3[0], sp3[1]))

        # 5. Articulation Joint Spheres
        # Base Mount
        painter.setPen(QPen(QColor(255, 255, 255, 200), 1.5))
        painter.setBrush(QBrush(QColor(15, 23, 42)))
        painter.drawEllipse(QPointF(sp0[0], sp0[1]), 5, 5)

        # Coxa Joint
        painter.setPen(QPen(col, 1.5))
        painter.setBrush(QBrush(QColor(30, 41, 59)))
        painter.drawEllipse(QPointF(sp1[0], sp1[1]), 6, 6)

        # Knee Joint
        painter.setPen(QPen(col, 2.0))
        painter.setBrush(QBrush(QColor(255, 255, 255)))
        painter.drawEllipse(QPointF(sp2[0], sp2[1]), 5, 5)

        # Foot Pad (Glowing contact point)
        foot_glow = QRadialGradient(sp3[0], sp3[1], 12.0)
        foot_glow.setColorAt(0.0, QColor(col.red(), col.green(), col.blue(), 255))
        foot_glow.setColorAt(0.5, QColor(col.red(), col.green(), col.blue(), 130))
        foot_glow.setColorAt(1.0, QColor(0, 0, 0, 0))
        painter.setPen(Qt.NoPen)
        painter.setBrush(QBrush(foot_glow))
        painter.drawEllipse(QPointF(sp3[0], sp3[1]), 9, 9)

        # Leg Name Tag
        painter.setPen(QPen(QColor(col.red(), col.green(), col.blue(), 220)))
        font = QFont("Helvetica", 9, QFont.Bold)
        painter.setFont(font)
        short_names = ["FR", "RR", "FL", "RL"]
        painter.drawText(QPointF(sp3[0] + 10, sp3[1] - 4), short_names[leg_idx])

    def _draw_hud(self, painter, w, h):
        # Controls info (Top-Left)
        painter.setPen(QPen(QColor(148, 163, 184, 180)))
        font = QFont("Helvetica", 9)
        painter.setFont(font)

        hud_text = f"Yaw: {self.yaw:.1f}° | Pitch: {self.pitch:.1f}° | Zoom: {self.zoom*100:.0f}%\n" \
                   f"L-Drag: Orbit | R-Drag/Wheel: Zoom | Shift-Drag: Pan | 2xClick: Reset"
        painter.drawText(QRectF(14, 14, w - 28, 40), Qt.AlignLeft | Qt.AlignTop, hud_text)

        # Live Robot Mode & Kinematics Stance Height (Bottom-Left)
        badge_rect = QRectF(14, h - 54, 240, 42)
        painter.setPen(QPen(QColor(0, 240, 255, 80), 1.0))
        painter.setBrush(QBrush(QColor(10, 15, 25, 220)))
        painter.drawRoundedRect(badge_rect, 6, 6)

        painter.setPen(QPen(QColor(0, 240, 255, 240)))
        font_badge = QFont("Helvetica", 10, QFont.Bold)
        painter.setFont(font_badge)
        painter.drawText(QRectF(22, h - 50, 220, 18), Qt.AlignLeft, f"MODE: {self.robot_mode}")

        painter.setPen(QPen(QColor(16, 185, 129 if self.pca_ready else 239, 68 if not self.pca_ready else 185, 68 if not self.pca_ready else 129)))
        font_sub = QFont("Helvetica", 8)
        painter.setFont(font_sub)
        status_text = "PCA9685 READY" if self.pca_ready else "PCA9685 OFFLINE"
        if self.moving:
            status_text += " [ACTION RUNNING]"
        if not self.telemetry_active:
            status_text = "OFFLINE (CONNECT TO VIEW LIVE)"
            painter.setPen(QPen(QColor(148, 163, 184)))
        painter.drawText(QRectF(22, h - 30, 220, 16), Qt.AlignLeft, status_text)
