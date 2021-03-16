//Matthew Wong
//CS344
//03/12/21
//assignment 6 this program provides a single threaded rust program that processes input numbers
//to produce a sum. The program contains extensive comments that explain the functionality and give
//directions on what parts of code you are allowed to change (look for comments starting with CHANGE CODE).


use std::env;                                                                                      // to get arugments passed to the program
use std::thread;                                                                                   //used to run threads

/*
* Print the number of partitions and the size of each partition
* @param vs A vector of vectors
*/
fn print_partition_info(vs: &Vec<Vec<usize>>)
{
    println!("Number of partitions = {}", vs.len());                                               //prints the nuber of partitions
    for i in 0..vs.len()                                                                           //loops the length of the index
    {
        println!("\tsize of partition {} = {}", i, vs[i].len());                                   //prints out the partitions we have
    }
}

/*
* Create a vector with integers from 0 to num_elements -1
* @param num_elements How many integers to generate
* @return A vector with integers from 0 to (num_elements - 1)
*/
fn generate_data(num_elements: usize) -> Vec<usize>
{
    let mut v : Vec<usize> = Vec::new();                                                           //create a vector with the size of -1 elements
    for i in 0..num_elements                                                                       //loop through the elements
    {
        v.push(i);                                                                                 //push the data into index slot for vector
    }
    return v;                                                                                      //return the vector we created above
}

/*
* Partition the data in the vector v into 2 vectors
* @param v Vector of integers
* @return A vector that contains 2 vectors of integers
*/
fn partition_data_in_two(v: &Vec<usize>) -> Vec<Vec<usize>>
{
    let partition_size = v.len() / 2;                                                              //set the partition_size to len of vector /2 for our two threads
    // Create a vector that will contain vectors of integers
    let mut xs: Vec<Vec<usize>> = Vec::new();                                                      //create a new vector
    // Create the first vector of integers
    let mut x1 : Vec<usize> = Vec::new();                                                          //create second vector
    // Add the first half of the integers in the input vector to x1
    for i in 0..partition_size                                                                     //loop through the size and push the data in
    {
        x1.push(v[i]);
    }
    // Add x1 to the vector that will be returned by this function
    xs.push(x1);
    // Create the second vector of integers
    let mut x2 : Vec<usize> = Vec::new();                                                          //do the same as above create vectors and loop
    // Add the second half of the integers in the input vector to x2
    for i in partition_size..v.len()                                                               //push data into vector 2
    {
        x2.push(v[i]);
    }
    // Add x2 to the vector that will be returned by this function
    xs.push(x2);
    // Return the result vector
    xs
}

/*
* Sum up the all the integers in the given vector
* @param v Vector of integers
* @return Sum of integers in v
* Note: this function has the same code as the reduce_data function.
*       But don't change the code of map_data or reduce_data.
*/
fn map_data(v: &Vec<usize>) -> usize
{
    let mut sum = 0;                                                                               //create variable for sum
    for i in v                                                                                     //loop until the vector end
    {
        sum += i;                                                                                  //sum up total in sum
    }
    sum
}

/*
* Sum up the all the integers in the given vector
* @param v Vector of integers
* @return Sum of integers in v
*/
fn reduce_data(v: &Vec<usize>) -> usize
{
    let mut sum = 0;                                                                               //create a sum variable
    for i in v                                                                                     //loop through
    {
        sum += i;                                                                                  //sum up the total
    }
    sum
}

/*
* A single threaded map-reduce program
*/
//generates data for the rest of the program
fn main()
{    // Use std::env to get arguments passed to the program
    let args: Vec<String> = env::args().collect();
    if args.len() != 3
    {
        println!("ERROR: Usage {} num_partitions num_elements", args[0]);                          //error message based on input
        return;
    }
    let num_partitions : usize = args[1].parse().unwrap();
    let num_elements : usize = args[2].parse().unwrap();
    if num_partitions < 1
    {
      println!("ERROR: num_partitions must be at least 1");                                        //error message based on input
        return;
    }
    if num_elements < num_partitions
    {
        println!("ERROR: num_elements cannot be smaller than num_partitions");                     //error validation based on input
        return;
    }
    //calls generate data to generate a vector of numbers that serves as input for the rest of the program
    // Generate data.
    let v = generate_data(num_elements);
    //calls partition which iwll partition the input numbers into two parititons
    // PARTITION STEP: partition the data into 2 partitions
    let xs = partition_data_in_two(&v);
    // Print info about the partitions
    print_partition_info(&xs);
    let mut intermediate_sums : Vec<usize> = Vec::new();
    // MAP STEP: Process each partition
    // CHANGE CODE START: Don't change any code above this line
    // Change the following code to create 2 threads that run concurrently and each of which uses map_data() function to process one of the two partitions
    //returns sum of all the numbers in that partition
    //intermediate_sums.push(map_data(&xs[0]));
    //intermediate_sums.push(map_data(&xs[1]));

    let xs_2 = xs.clone();                                                                         //clone xs to use with the threads

    let thread_1 = thread::spawn(move || map_data(&xs[0]));                                        //start 2 threads and pass xs data std::thread
    let thread_2 = thread::spawn(move || map_data(&xs_2[1]));

    let return_1 = thread_1.join().unwrap();                                                       //join threads
    let return_2 = thread_2.join().unwrap();

    intermediate_sums.push(return_1);                                                              //push results
    intermediate_sums.push(return_2);


    // CHANGE CODE END: Don't change any code below this line until the next CHANGE CODE comment

    // Print the vector with the intermediate sums
    println!("Intermediate sums = {:?}", intermediate_sums);

    //sums the intermediate results produced by the map step to produce the final step of all the input numbers ß
    // REDUCE STEP: Process the intermediate result to produce the final result
    let sum = reduce_data(&intermediate_sums);
    println!("Sum = {}", sum);

    // CHANGE CODE: Add code that does the following:
    // 1. Calls partition_data to partition the data into equal partitions
    let equal_partition = partition_data(num_partitions, &v);
    // 2. Calls print_partition_info to print info on the partitions that have been created
    print_partition_info(&equal_partition);
    // 3. Creates one thread per partition and uses each thread to concurrently process one partition
    //vector for each of the sums created by the threads
    let mut intermediate_sums_2 : Vec<usize> = Vec::new();
    // 4. Collects the intermediate sums from all the threads

        //run map_data function for partitions
        for t in 0..num_partitions
        {
            //use a clone for each vector
            let new_clone = equal_partition.clone();
            //make a new thread and pass the clone in
            let new_thread = thread::spawn(move || map_data(&new_clone[t]));
            let return_thread = new_thread.join().unwrap();
            //push to the vector
            intermediate_sums_2.push(return_thread);
        }
    // 5. Prints information about the intermediate sums
    println!("intermediate sums = {:?}",intermediate_sums_2);
    // 5. Calls reduce_data to process the intermediate sums
    //process our info with reduce
    let second_sum = reduce_data(&intermediate_sums_2);
    // 6. Prints the final sum computed by reduce_data
    println!("Sum = {}", second_sum);

}

/*
* CHANGE CODE: code this function
* Note: Don't change the signature of this function
*
* Partitions the data into a number of partitions such that
* - the returned partitions contain all elements that are in the input vector
* - if num_elements is a multiple of num_partitions, then all partitions must have equal number of elements
* - if num_elements is not a multiple of num_partitions, some partitions can have one more element than other partitions
*
* @param num_partitions The number of partitions to create
* @param v The data to be partitioned
* @return A vector that contains vectors of integers
*
*/
fn partition_data(num_partitions: usize, v: &Vec<usize>) -> Vec<Vec<usize>>
{
    //Remove the following line which has been added to remove a compiler error
    //partition_data_in_two(v)
    //use the number of partitions passed in to set size
    let partition_size = v.len() / num_partitions;
    let mut xs: Vec<Vec<usize>> = Vec::new();

    //used for loop calculations later on
    let mut modded_value = v.len() % num_partitions;

    //use a loop to go through until partition size
    let mut start_num = 0;
    let mut end_num = partition_size;

    //loop through the partition
    for i in 0..num_partitions
    {
        //push
        let mut x : Vec<usize> = Vec::new();
        for j in start_num..end_num
        {
            x.push(v[j]);
        }
        xs.push(x);

        //mod > 0 == partition size is not even
        //+ 1 to vector and keep while dec
        // mod until mod is 0

        if modded_value > 0
        {
            xs[i].push(v[end_num]);
            start_num = start_num + partition_size +1;
            end_num = end_num + partition_size + 1;
            modded_value -= 1;
        }
        else
        {
            start_num = start_num + partition_size;
            end_num = end_num + partition_size;
        }
    }
    //pass the final completed vector back up
    xs
}
