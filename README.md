### systems-monitor - Simple Remote Systems Monitoring Tool

## Supported OS:
*Linux (Tested on Ubuntu)

## Usage:

# Build the source:
```
make
```

# Start daemon on server:
```
make run-server &
```

# Start daemon on client:
```
make run-client &
```

# Add clients
Add clients(ipv4 address) to config/clients.txt

# Setting Up the Dashboard on server:
1. Install Apache server
   ```
   sudo apt install apache2
   ```
2. Create symbolic link to the "site" directory
   ```
   sudo ln -s /path/to/site/directory /var/www/html/systems-monitor
   ```
3. Launch the Dashboard - http://server_ip_address/systems-monitor

