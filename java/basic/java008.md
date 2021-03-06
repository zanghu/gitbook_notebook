### service、domain、entity和DAO的使用场景

参考资料：[CSDN: domain、entity和model的package的不同用法](https://blog.csdn.net/id_rin/article/details/54288809)


鼓励使用service domain dao 层分层设计概念。

其次，对几层作用的理解：


第一：dao层操作单表，不涉及复杂逻辑，主要是表的增删改查操作，完全根据domain的要求来查询数据。

第二：domain层考虑业务逻辑，例如过滤条件，放行或者返回，以及数据的处理，为调用dao层做好准备，一个domain可以调用一个或者一组相关的dao层

第三：service层调用一个或者一组domain层，主要是展现需要开放出去的接口，其中domain层不是所有的接口都要再service层体现的，可能仅在service层开放几个接口出去，此外，主要接口需要对接受的参数要尽量的扩大化，也就是说可以容纳各种类型的参数的接入（Object），然后需要在service层做好转换，以备domain层使用。

 

正确的设计应该是，一个领域活动会聚合对应一个或一组DAO ，来完成一个领域活动。而一个服务可能包含两个领域活动，比如一个转账的业务，对应两个领域活动。两个帐户的金额分别发生变化，需要操作一组领域活动，而每个活动需要操作很多表（调用多个DAO ）。