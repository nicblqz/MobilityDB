import matplotlib.pyplot as plt
from dateutil import parser
import csv

def read_trajectory_data(filename):
    trajectories = {}
    
    with open(filename, 'r') as file:
        reader = csv.reader(file)
        for row in reader:
            if len(row) == 6:  # assuming each row has 6 columns as per your example
                timestamp_str = row[0].rstrip('+01')  # remove +01 from the end of the timestamp
                tid = int(row[1])
                latitude = float(row[2])
                longitude = float(row[3])
                # speed = float(row[4])  # ignoring speed for this example
                # angle = float(row[5])  # ignoring angle for this example
                
                # Parse timestamp string with dateutil.parser
                timestamp = parser.parse(timestamp_str)
                
                # Store trajectory data by tid
                if tid in trajectories:
                    trajectories[tid]['timestamps'].append(timestamp)
                    trajectories[tid]['latitudes'].append(latitude)
                    trajectories[tid]['longitudes'].append(longitude)
                else:
                    trajectories[tid] = {
                        'timestamps': [timestamp],
                        'latitudes': [latitude],
                        'longitudes': [longitude]
                    }
    
    return trajectories

def plot_trajectories(trajectories):
    plt.figure(figsize=(10, 8))
    
    # Define colormap for 70 different tids
    colormap = plt.cm.get_cmap('tab20', 70)
    
    for idx, (tid, data) in enumerate(trajectories.items()):
        if idx >= 70:  # Limit to 70 trajectories
            break
        timestamps = data['timestamps']
        latitudes = data['latitudes']
        longitudes = data['longitudes']
        
        # Plot trajectory for each tid with different color
        plt.scatter(longitudes, latitudes, c=[colormap(idx)] * len(longitudes), label=f'TID {tid}', alpha=0.75)
    
    plt.colorbar(label='Trajectory ID')
    plt.xlabel('Longitude')
    plt.ylabel('Latitude')
    plt.title('Trajectories')
    #plt.legend(loc='upper right', bbox_to_anchor=(1.15, 1.05))
    plt.grid(True)
    plt.tight_layout()
    plt.show()

# Example usage
filename = 'compressed_output.csv'  # Replace with your actual file path
trajectories = read_trajectory_data(filename)
plot_trajectories(trajectories)

