# Progetto-MPSR
Performance Modeling of Computer Systems and Networks
Prof. V. de Nitto Persone’
AA 2014/ 2015 Project 9CFU

Consider a system that provides web services and which is built according to the widely used multi-tiered paradigm. Typically, the multi-tier application consists of a web server, an application server, and a back-end database. The web server and the application server reside usually within the same physical server, which is called front server. Access to a web service occurs in the form of a session consisting of many individual requests. After a new session connection is generated, client requests circulate among the front and database server before they are sent back to the client. After a request is sent back, the client spends an average think time E[Z] before sending the following request. A session completes after the client has generated a series of requests.For a customer trying to place an order, or a retailer trying to make a sale, the real measure of a web server performance is its ability to process the entire sequence of requests needed to complete a transaction. The useful throughput of the system is measured as a function of the number of completed sessions. Aborted requests of already accepted sessions are highly undesirable because they compromise the server’s ability to process all requests needed to complete a transaction and result in wasted system resources. The percentage of refused new connection requests in respect of the total arrival flow is called new session drop ratio. The percentage of refused requests in respect of the total requests of existing sessions (on-line requests) is called aborted ratio.

Consider a model for the described system with the following parameters:

* inter-arrival times of new session requests exponentially distributed with rate 35 requests/s;
* session length Uniform(5, 35);
* E[Z]=7 s exponentially distributed;
* E[D]front server=0.00456 s exponentially distributed;
* E[D]backend server=0.00117s exponentially distributed.

1. Evaluate the steady state system response time, the useful throughput and the response time sample autocorrelation rj for j=1, 2, ..., 20.
2. Determine the sample mean histogram for one of the computed statistics.
3. Consider the following two different distributions for the front-server service time: 10-Erlang and Hyperexponential with p=0.1. By assuming the same mean as for the exponential case, evaluate the steady state behaviour of the system and compare with the results in 1.
4. For the system with the “worst” performance, implement an overload management mechanism based on monitoring the CPU utilization of the web server as follows: if the observed utilization is above 85%, then for the next time interval the admission controller rejects any request. The system resumes the “normal” operation condition when its utilization drops below the 75%. Evaluate the system response time, the useful throughput, the drop ratio and the aborted ratio.

All the results must be obtained with a 95% confidence level.
