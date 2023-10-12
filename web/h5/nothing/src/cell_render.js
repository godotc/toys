
export const CellRender = (device, canvasFormat) => {

    const WORKGROUP_SIZE = 8;
    const GRID_SIZE = 16;



    /**
     * Create the shader module(code), and specify  its layout,
     */
    const { RenderShaderModule, SimulationShaderModule, BindGroupLayout } = (() => {

        // shader
        const RenderShaderModule = device.createShaderModule({
            label: "Cell shader",
            code: `
                @group(0) @binding(0) var<uniform> grid: vec2f;
                @group(0) @binding(1) var<storage> cellState: array<u32>;

                //----------------------

                struct VertexInput{
                    @builtin(instance_index) instance: u32,
                    @location(0) pos: vec2f,
                };

                struct VertexOutput{
                    @builtin(position) pos: vec4f,
                    @location(0) cell: vec2f,
                }


                @vertex
                fn vertexMain(input: VertexInput)  -> VertexOutput {

                    let i = f32(input.instance);

                    let cell = vec2f( i % grid.x, floor(i/ grid.x));
                    let state =f32(cellState[input.instance]);

                    let cellOffset = cell/ grid * 2;
                    let gridPos = (input.pos * state + 1) /grid -1 + cellOffset;

                    var o : VertexOutput;
                    o.pos = vec4f(gridPos, 0, 1);
                    o.cell = cell;
                    return o;

                }

                //---------------------------------

                @fragment
                fn fragmentMain(input: VertexOutput) -> @location(0) vec4f{
                    let c = input.cell/ grid;
                    return vec4f(c, 1-c.x, 1);
                }
            `
        });

        // compute shader
        const SimulationShaderModule = device.createShaderModule({
            label: "Cell Compute shader",
            code: `
                @group(0) @binding(0) var<uniform> grid: vec2f;

                @group(0) @binding(1) var<storage> cellStateIn: array<u32>;
                @group(0) @binding(2) var<storage, read_write> cellStateOut: array<u32>;

                fn cellIndex(cell: vec2u) -> u32 {
                    return  (cell.y % u32(grid.y)) * u32(grid.x)
                            + (cell.x % u32(grid.x));
                }

                fn cellActive(x: u32, y: u32) -> u32{
                    return  cellStateIn[cellIndex(vec2( x,y))] ;
                }

                @compute @workgroup_size(${WORKGROUP_SIZE}, ${WORKGROUP_SIZE})
                fn computeMain(@builtin(global_invocation_id) cell: vec3u) {

                    var activeNeighbors:u32  = 0;
                    for  ( var i = -1; i <2 ; i++) {
                        for  ( var j = -1; j <2 ; j++) {
                            activeNeighbors +=  cellActive( cell.x + u32(i), cell.y + u32(j) );
                        }
                    }
                    if( cellActive(cell.x, cell.y) == 1){
                        activeNeighbors -=1;
                    } 

                    let i = cellIndex(cell.xy);

                    switch activeNeighbors {
                        case 2: {
                            cellStateOut[i] = 1;
                        }
                        case 3: {
                            cellStateOut[i] = cellStateIn[i];
                        }
                        default: {
                            cellStateOut[i] = 0;
                        }
                    }
                }
            `
        });

        // Create the bind group layout and pipeline layout.
        // Now we create a common layout  for 2 shader
        const bindGroupLayout = device.createBindGroupLayout({
            label: "Cell Bind Group Layout",
            entries: [{
                binding: 0,
                visibility: GPUShaderStage.VERTEX | GPUShaderStage.FRAGMENT | GPUShaderStage.COMPUTE, //unifrom buffer add usage for fragment shader
                buffer: {} // Grid uniform buffer
            }, {
                binding: 1,
                visibility: GPUShaderStage.VERTEX | GPUShaderStage.COMPUTE,
                buffer: { type: "read-only-storage" } // Cell state input buffer
            }, {
                binding: 2,
                visibility: GPUShaderStage.COMPUTE,
                buffer: { type: "storage" } // Cell state output buffer
            }]
        });


        return { RenderShaderModule, SimulationShaderModule, BindGroupLayout: bindGroupLayout };

    })();


    /**
     *  Vertex Buffer
      * 1. define the initial data that we input as vertices
      * 2. create the corresponing buffer  (this for vertex)
      * 3. define its layout
      */
    const { Vertices, VertexBuffer, VertexBufferLayout } = (() => {
        // vertex
        const vertices = new Float32Array([
            //   X,    Y,
            -0.8, -0.8, // Triangle 1 (Blue)
            0.8, -0.8,
            0.8, 0.8,

            -0.8, -0.8, // Triangle 2 (Red)
            0.8, 0.8,
            -0.8, 0.8,
        ]);

        const vertexBuffer = device.createBuffer({
            label: "Cell Vertices",
            size: vertices.byteLength,
            usage: GPUBufferUsage.VERTEX | GPUBufferUsage.COPY_DST,
        })
        device.queue.writeBuffer(vertexBuffer, 0, vertices);

        const vertexBufferLayout = {
            arrayStride: 8,
            attributes: [{
                format: "float32x2",
                offset: 0,
                shaderLocation: 0,
            }]
        }

        return { Vertices: vertices, VertexBuffer: vertexBuffer, VertexBufferLayout: vertexBufferLayout }
    })();


    /**
     * Create 3 more buffers...
     * 2 is storage,  with same size for the swap between compute and render
     * 1 is unifrom on the  fix layout(0)
     */
    const { CellStateArray, CellStateStorage, UniformArray, UniformBuffer } = (() => {

        /**
         * Storage buffer
         */
        let cellStateArray, cellStateStorage;
        {
            cellStateArray = new Uint32Array(GRID_SIZE * GRID_SIZE);

            // create buffer on gpu
            cellStateStorage = [
                device.createBuffer({
                    label: "Cell State A",
                    size: cellStateArray.byteLength,
                    usage: GPUBufferUsage.STORAGE | GPUBufferUsage.COPY_DST,
                }),
                device.createBuffer({
                    label: "Cell State B",
                    size: cellStateArray.byteLength,
                    usage: GPUBufferUsage.STORAGE | GPUBufferUsage.COPY_DST,
                })
            ];

            for (let i = 0; i < cellStateArray.length; i += 3) {
                cellStateArray[i] = Math.random() > 0.6 ? 1 : 0;
            }
            device.queue.writeBuffer(cellStateStorage[0], 0, cellStateArray)

            for (let i = 0; i < cellStateArray.length; i += 3) {
                cellStateArray[i] = i % 2;
            }
            device.queue.writeBuffer(cellStateStorage[1], 0, cellStateArray)
        }


        /**
         * Unifrom buffer
         */
        let uniformArray, uniformBuffer;
        {
            uniformArray = new Float32Array([GRID_SIZE, GRID_SIZE])

            uniformBuffer = device.createBuffer({
                label: "Grid Unifroms",
                size: uniformArray.byteLength,
                usage: GPUBufferUsage.UNIFORM | GPUBufferUsage.COPY_DST
            });
            device.queue.writeBuffer(uniformBuffer, 0, uniformArray);
        }


        return {
            CellStateArray: cellStateArray,
            CellStateStorage: cellStateStorage,
            UniformArray: uniformArray,
            UniformBuffer: uniformBuffer
        };

    })();


    /**
     * The bind group, specific for the each loaction's corresponding buffer
     * follow the bind group layout in shader 
     * We can create differenet layout with different ordering buffer
     */
    const BindGroups = [
        device.createBindGroup({
            label: "Cell renderer bind group A",
            layout: BindGroupLayout,
            entries: [
                {
                    binding: 0,
                    resource: { buffer: UniformBuffer }
                },
                {
                    binding: 1,
                    resource: { buffer: CellStateStorage[0] }
                },
                {
                    binding: 2,
                    resource: { buffer: CellStateStorage[1] }
                }
            ],
        }),
        device.createBindGroup({
            label: "Cell renderer bind group B",
            layout: BindGroupLayout,
            entries: [
                {
                    binding: 0,
                    resource: { buffer: UniformBuffer }
                },
                {
                    binding: 1,
                    resource: { buffer: CellStateStorage[1] }
                },
                {
                    binding: 2,
                    resource: { buffer: CellStateStorage[0] }
                }
            ],
        }),
    ];

    /**
     * Create 2 pipeline from the `BindGroupLayout`
     */
    const { RenderPipeline, SimulationPipeline } = (() => {

        /**
         * Pipeline layout for create pipeline, so it can be used together 
         * because the common bindGroupLayout(and shader).
         * 
         * Maybe need different pipeline layout when render and compute do different context work
         */
        const pipelineLayout = device.createPipelineLayout({
            label: "Cell Pipeline Layout",
            bindGroupLayouts: [BindGroupLayout] // the group(0)
        });

        // pipeline
        const RenderPipeline = device.createRenderPipeline({
            label: "Cell pipelinie",
            layout: pipelineLayout,
            vertex: {
                module: RenderShaderModule,
                entryPoint: "vertexMain",
                buffers: [VertexBufferLayout]
            },
            fragment: {
                module: RenderShaderModule,
                entryPoint: "fragmentMain",
                targets: [{
                    format: canvasFormat
                }]
            },
        })

        const simulationPipeline = device.createComputePipeline({
            label: "Simulation pipeline",
            layout: pipelineLayout,
            compute: {
                module: SimulationShaderModule,
                entryPoint: "computeMain"
            }
        })

        return { RenderPipeline, SimulationPipeline: simulationPipeline }

    })();




    return new class {
        GridSize = GRID_SIZE
        WorkGroupSize = WORKGROUP_SIZE

        CellPipeline = RenderPipeline
        SimulationPipeline = SimulationPipeline

        Vertices = Vertices
        VertexBuffer = VertexBuffer

        BindGroup = BindGroups


        Compute = (compute_pass, step) => {
            compute_pass.setPipeline(this.SimulationPipeline);
            compute_pass.setBindGroup(0, this.BindGroup[step % 2]);

            const workgroupCount = Math.ceil(this.GridSize / this.WorkGroupSize)
            compute_pass.dispatchWorkgroups(workgroupCount, workgroupCount);
        }

        Draw = (render_pass, step) => {

            render_pass.setPipeline(this.CellPipeline);
            render_pass.setBindGroup(0, this.BindGroup[step % 2]);

            render_pass.setVertexBuffer(0, this.VertexBuffer)

            render_pass.draw(this.Vertices.length / 2, this.GridSize * this.GridSize);

        }
    }
};
