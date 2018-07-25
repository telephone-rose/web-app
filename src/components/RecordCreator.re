type fileUploadData = {
  id: string,
  uploadUrl: string,
};

module RequestFileUpload = [%graphql
  {|
    mutation RequestFileUpload (
      $contentLength: Int!
      $contentType: String!
    ) {
      requestFileUpload (
        contentLength: $contentLength
        contentType: $contentType
      ) @bsRecord {
        id
        uploadUrl
      }
    }
  |}
];

module RequestFileUploadMutation =
  ReasonApollo.CreateMutation(RequestFileUpload);

type compressedFile = {downloadUrl: string};

type transcriptWord = {
  endTime: string,
  startTime: string,
  word: string,
};

type recording = {
  id: string,
  compressedFile,
  transcript: string,
  transcriptWords: array(transcriptWord),
};

module CreateRecording = [%graphql
  {|
      mutation CreateRecording (
        $fileId: ID!
        $languageCode: language!
      ) {
        createRecording (
          fileId: $fileId,
          languageCode: $languageCode
        ) @bsRecord {
          id
          compressedFile @bsRecord {
            downloadUrl
          }
          transcript
          transcriptWords @bsRecord {
            endTime
            startTime
            word
          }
        }
      }
    |}
];

module CreateRecordingMutation = ReasonApollo.CreateMutation(CreateRecording);

let component = ReasonReact.statelessComponent("RecordCreator");

let make = (~onRecordingCreation, ~onRecordingError, _children) => {
  ...component,
  render: _self =>
    <CreateRecordingMutation>
      ...(
           (createRecording, _) =>
             <RequestFileUploadMutation>
               ...(
                    (mutate, _) =>
                      <AudioRecorder
                        onRecordReady=(
                          recordingBlob => {
                            let fileAttributes =
                              RequestFileUpload.make(
                                ~contentLength=
                                  recordingBlob |> OpusRecorder.getBlobSize,
                                ~contentType=
                                  recordingBlob |> OpusRecorder.getBlobType,
                                (),
                              );
                            Js.Promise.(
                              mutate(~variables=fileAttributes##variables, ())
                              |> then_(response =>
                                   resolve(
                                     RequestFileUploadMutation.convertJsInputToReason(
                                       response,
                                     ),
                                   )
                                 )
                              |> then_(
                                   (
                                     response: RequestFileUploadMutation.renderPropObj,
                                   ) => {
                                   switch (response.data) {
                                   | Some(data) =>
                                     let requestFileUploadResult = data##requestFileUpload;
                                     Fetch.fetchWithInit(
                                       requestFileUploadResult.uploadUrl,
                                       Fetch.RequestInit.make(
                                         ~method_=Put,
                                         ~body=
                                           Fetch.BodyInit.makeWithBlob(
                                             recordingBlob,
                                           ),
                                         (),
                                       ),
                                     )
                                     |> then_(_ => {
                                          let createRecordingParams =
                                            CreateRecording.make(
                                              ~fileId=
                                                requestFileUploadResult.id,
                                              ~languageCode=`frFR,
                                              (),
                                            );

                                          createRecording(
                                            ~variables=createRecordingParams##variables,
                                            (),
                                          );
                                        })
                                     |> then_(response =>
                                          resolve(
                                            CreateRecordingMutation.convertJsInputToReason(
                                              response,
                                            ),
                                          )
                                        )
                                     |> then_(
                                          (
                                            response: CreateRecordingMutation.renderPropObj,
                                          ) => {
                                          switch (response.data) {
                                          | Some(data) =>
                                            let createRecordingResult = data##createRecording;
                                            onRecordingCreation(
                                              createRecordingResult,
                                            );
                                          | None => ()
                                          };
                                          resolve();
                                        })
                                     |> catch(_ => {
                                          onRecordingError();
                                          resolve();
                                        })
                                     |> ignore;
                                     ();
                                   | _ => ()
                                   };
                                   resolve();
                                 })
                              |> ignore
                            );
                            ();
                          }
                        )
                      />
                  )
             </RequestFileUploadMutation>
         )
    </CreateRecordingMutation>,
};
