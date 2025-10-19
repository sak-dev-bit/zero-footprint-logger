# Zero-Footprint Kernel Metric Logger

A highly efficient, single-file C application designed to read low-level Linux kernel metrics, containerized using a multi-stage Docker build, and deployed with minimal Kubernetes primitives. This project demonstrates deep expertise in **C/C++ systems programming**, **Linux internals**, and **modern DevOps practices**.

-----

## 🚀 Project Goal

The primary goal was to create a **minimal, high-performance systems tool** that showcases the complete lifecycle of a metric logger: from reading system data at the kernel level to orchestrated deployment on a container platform. The focus was on **zero external dependencies** and the **smallest possible final footprint**.

-----

## 🛠 Technology Stack & Key Achievements

| Area | Technology Used | Key Achievement Demonstrated |
| :--- | :--- | :--- |
| **Systems / Kernel** | **C Programming** (single file) and **Linux Internals** (`/proc` filesystem) | Implemented direct file I/O to read real-time host status from the kernel's virtual filesystem (`/proc/uptime`), proving deep knowledge of Unix/Linux core mechanisms. |
| **Containerization** | **Docker** (Multi-Stage Build) | Containerized the C binary using a multi-stage build (`gcc:11` $\rightarrow$ `alpine:latest`), resulting in an **extremely small final image** for resource efficiency. |
| **Orchestration** | **Kubernetes** (Pod, `hostPath` Volume) | Deployed the logger using the minimal K8s object (**Pod**) and utilized a **`hostPath` volume mount** to expose the host's `/proc` filesystem to the container, a critical pattern for host-level monitoring agents. |

-----

## 📂 Project Structure

```
zero-footprint-logger/
├── main.c              # The core C application (reads /proc/uptime)
├── Dockerfile          # Multi-stage build for minimal image size
├── logger-pod.yaml     # Kubernetes Pod manifest for deployment
└── README.md           # Project documentation (this file)
```

-----

## 💻 Detailed Implementation

### 1\. The Core Logger (`main.c`)

The C program's design is intentionally simple:

  * **Reads** the system uptime directly from `/proc/uptime`.
  * **Parses** the value (uptime in seconds).
  * **Prints** the result to `stdout` (which is captured by the container runtime/Kubernetes logs).
  * **Exits immediately** with status code `0`.

### 2\. Multi-Stage Docker Build (`Dockerfile`)

This ensures the final image contains **only the necessary binary**, not the compiler tools.

1.  **Stage 1 (Builder):** Uses the large `gcc:11` image to compile `main.c` into a statically linked `metric_logger` binary.
2.  **Stage 2 (Runtime):** Uses the tiny `alpine:latest` image and copies **only** the compiled binary, achieving a footprint in the low megabytes.

### 3\. Kubernetes Deployment (`logger-pod.yaml`)

The Pod definition is structured for a monitoring agent:

  * **`hostPath` Volume:** The host's `/proc` directory is mounted into the container, allowing the containerized logger to read the host machine's system metrics.
  * **`restartPolicy: OnFailure`:** Since the C program is designed to run once and exit, this policy is set to restart the container automatically (e.g., every $\sim30$ seconds), simulating a metric collection interval.

-----

## ⚙️ Build and Deploy Instructions

### Prerequisites

  * **`gcc`** (for local testing)
  * **`docker`**
  * **`kubectl`** (and a running K8s cluster like Minikube/Docker Desktop)

### 1\. Build and Push the Docker Image

Replace `<YOUR_DOCKER_HUB_USERNAME>` with your Docker Hub ID.

```bash
# 1. Compile locally (for testing)
gcc main.c -o metric_logger
./metric_logger

# 2. Build the multi-stage image
docker build -t <YOUR_DOCKER_HUB_USERNAME>/c-metric-logger:latest .

# 3. Push the image to Docker Hub
docker push <YOUR_DOCKER_HUB_USERNAME>/c-metric-logger:latest
```

### 2\. Deploy to Kubernetes

Ensure the `image` field in `logger-pod.yaml` is updated with your Docker Hub username, then deploy:

```bash
# Apply the Pod manifest
kubectl apply -f logger-pod.yaml

# Check the Pod status (it will cycle through Running -> Completed -> restart)
kubectl get pods

# View the captured metric logs
kubectl logs uptime-logger -f
```

**Output in logs:**

```
System Uptime: 12345.67 seconds
... (Pod restarts)
System Uptime: 12375.12 seconds
...
```

### 3\. Clean Up

```bash
kubectl delete pod uptime-logger
```
