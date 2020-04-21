# This will get updated when the datasource gets changed.
# Visit https://data.world/ to get the new url

COVID_URL=https://query.data.world/s/7cpqb7nca6bvh7nz6tdwdrfdaz6k4a

fetchData:
	wget -O bin/data/covid_data.csv $(COVID_URL)
