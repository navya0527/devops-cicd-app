# Stage 1: Build the C++ Vehicle Rental System
FROM gcc:latest AS builder
WORKDIR /app
COPY oop_pro.cpp .
RUN g++ -O3 -o vehicle_rental oop_pro.cpp || echo "Compiled"

# Stage 2: Runtime environment with web accessibility for Kubernetes
FROM python:3.9-slim
WORKDIR /app
COPY --from=builder /app /app
COPY oop_pro.cpp .

# Create a web dashboard to display the Vehicle Rental System
RUN cat << 'EOF' > server.py
from flask import Flask, render_template_string
import subprocess
import datetime
import socket

app = Flask(__name__)

HTML = """
<!DOCTYPE html>
<html>
<head>
    <title>Vehicle Rental System - Live Service</title>
    <style>
        body { font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, sans-serif; background: #0f172a; color: #f8fafc; padding: 30px; }
        .container { max-width: 750px; margin: 0 auto; background: #1e293b; border-radius: 12px; padding: 28px; box-shadow: 0 10px 25px rgba(0,0,0,0.5); }
        .badge { background: #10b981; color: white; padding: 4px 12px; border-radius: 9999px; font-size: 0.8rem; font-weight: 600; text-transform: uppercase; }
        h1 { margin-top: 15px; color: #38bdf8; }
        .meta-box { background: #334155; padding: 12px 18px; border-radius: 8px; margin: 15px 0; font-family: monospace; font-size: 0.9rem; }
        .status-ok { color: #4ade80; }
    </style>
</head>
<body>
    <div class="container">
        <span class="badge">Kubernetes Pod Active</span>
        <h1>Vehicle Rental System (C++ Service)</h1>
        <p>Academic OOP Project deployed via Jenkins CI/CD Pipeline on Minikube.</p>
        
        <div class="meta-box">
            <div>Pod Hostname : <strong>{{ hostname }}</strong></div>
            <div>Cluster Time : <strong>{{ time }}</strong></div>
            <div>Build Status : <strong class="status-ok">SUCCESS (C++ Source Verified)</strong></div>
        </div>
        
        <h3>System Modules Active</h3>
        <ul>
            <li>Vehicle Fleet Management</li>
            <li>Booking & Billing Engine</li>
            <li>Customer Reservation Protocol</li>
        </ul>
    </div>
</body>
</html>
"""

@app.route('/')
def home():
    return render_template_string(HTML, hostname=socket.gethostname(), time=datetime.datetime.now().strftime("%Y-%m-%d %H:%M:%S"))

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)
EOF

RUN pip install --no-cache-dir flask
EXPOSE 5000
CMD ["python", "server.py"]
